{-
-- EPITECH PROJECT, 2025
-- GLaDOS VM
-- File description:
-- Virtual Machine
-}

module VM (runVM, runProgram) where

import Types
import qualified Data.Map as Map

runProgram :: [Instruction] -> Map.Map String ([String], [Instruction]) -> Either String Value
runProgram mainCode funcs = do
    let state = initVM { functions = funcs }
    finalState <- runVM mainCode state
    case stack finalState of
        (v:_) -> Right v
        [] -> Right (VInt 0)  -- Default return value

runVM :: [Instruction] -> VMState -> Either String VMState
runVM instructions state
    | pc state >= length instructions = Right state
    | otherwise = do
        let instr = instructions !! pc state
        newState <- executeInstruction instr instructions state
        runVM instructions newState

executeInstruction :: Instruction -> [Instruction] -> VMState -> Either String VMState

-- Push on stack
executeInstruction (Push val) _ state = 
    Right $ state 
        { stack = val : stack state
        , pc = pc state + 1
        }

-- Pop from stack
executeInstruction Pop _ state = 
    case stack state of
        [] -> Left "Stack underflow"
        (_:xs) -> Right $ state 
            { stack = xs
            , pc = pc state + 1
            }

-- Load variabile
executeInstruction (LoadVar name) _ state =
    case Map.lookup name (vars state) of
        Nothing -> Left $ "Variable not found: " ++ name
        Just val -> Right $ state
            { stack = val : stack state
            , pc = pc state + 1
            }

-- Store variabile
executeInstruction (StoreVar name) _ state =
    case stack state of
        [] -> Left "Cannot store: empty stack"
        (val:xs) -> Right $ state
            { stack = xs
            , vars = Map.insert name val (vars state)
            , pc = pc state + 1
            }

-- Binary operation
executeInstruction (BinOp op) _ state =
    case stack state of
        (right:left:xs) -> do
            result <- applyOp op left right
            Right $ state
                { stack = result : xs
                , pc = pc state + 1
                }
        _ -> Left "Not enough operands for binary operation"

-- Conditional jump
executeInstruction (JumpIfFalse offset) _ state =
    case stack state of
        [] -> Left "Cannot jump: empty stack"
        (val:xs) -> jumpIfFalseLogic val xs offset state

-- Unconditional jump
executeInstruction (Jump offset) _ state =
    Right $ state { pc = pc state + offset + 1 }

-- Function call
executeInstruction (CallFunc name argCount) _ state =
    case Map.lookup name (functions state) of
        Nothing -> Left $ "Function not found: " ++ name
        Just (params, funcBody) ->
            if length params /= argCount
                then Left $ "Wrong number of arguments for " ++ name
                else callFuncLogic params funcBody argCount state

-- Return
executeInstruction Return instructions state =
    Right $ state { pc = length instructions }

-- Load args
executeInstruction (LoadArg idx) _ state =
    case Map.toList (vars state) of
        [] -> Left "No arguments available"
        pairs -> loadArgLogic idx pairs state

jumpIfFalseLogic :: Value -> [Value] -> Int -> VMState -> Either String VMState
jumpIfFalseLogic (VBool False) xs offset state =
    Right $ state { stack = xs, pc = pc state + offset + 1 }
jumpIfFalseLogic (VBool True) xs _ state =
    Right $ state { stack = xs, pc = pc state + 1 }
jumpIfFalseLogic (VInt 0) xs offset state =
    Right $ state { stack = xs, pc = pc state + offset + 1 }
jumpIfFalseLogic (VInt _) xs _ state =
    Right $ state { stack = xs, pc = pc state + 1 }


callFuncLogic :: [String] -> [Instruction] -> Int -> VMState -> Either String VMState
callFuncLogic params funcBody argCount state = do
    let (args, restStack) = extractArgs argCount state
    let localEnv = prepareLocalEnv params args state
    let funcState = createFuncState localEnv state
    finalState <- runVM funcBody funcState
    processFuncResult finalState restStack state

extractArgs :: Int -> VMState -> ([Value], [Value])
extractArgs argCount state =
    let args = reverse $ take argCount (stack state)
        restStack = drop argCount (stack state)
    in (args, restStack)

prepareLocalEnv :: [String] -> [Value] -> VMState -> Map.Map String Value
prepareLocalEnv params args state =
    let localParams = Map.fromList (zip params args)
    in Map.union localParams (vars state)

createFuncState :: Map.Map String Value -> VMState -> VMState
createFuncState localEnv state = VMState
    { stack = []
    , vars = localEnv
    , functions = functions state
    , pc = 0
    }

processFuncResult :: VMState -> [Value] -> VMState -> Either String VMState
processFuncResult finalState restStack state =
    case stack finalState of
        (result:_) -> Right $ state
            { stack = result : restStack, pc = pc state + 1 }
        [] -> Right $ state
            { stack = VInt 0 : restStack, pc = pc state + 1 }


loadArgLogic :: Int -> [(String, Value)] -> VMState -> Either String VMState
loadArgLogic idx pairs state =
    if idx < length pairs
        then let (_, val) = pairs !! idx
             in Right $ state
                { stack = val : stack state
                , pc = pc state + 1
                }
        else Left $ "Argument index out of bounds: " ++ show idx

applyOp :: Op -> Value -> Value -> Either String Value
applyOp Add (VInt a) (VInt b) = Right $ VInt (a + b)
applyOp Sub (VInt a) (VInt b) = Right $ VInt (a - b)
applyOp Mul (VInt a) (VInt b) = Right $ VInt (a * b)
applyOp Div (VInt _) (VInt 0) = Left "Division by zero"
applyOp Div (VInt a) (VInt b) = Right $ VInt (a `div` b)
applyOp Eq (VInt a) (VInt b) = Right $ VBool (a == b)
applyOp Eq (VBool a) (VBool b) = Right $ VBool (a == b)
applyOp NotEq (VInt a) (VInt b) = Right $ VBool (a /= b)
applyOp NotEq (VBool a) (VBool b) = Right $ VBool (a /= b)
applyOp Less (VInt a) (VInt b) = Right $ VBool (a < b)
applyOp Greater (VInt a) (VInt b) = Right $ VBool (a > b)
applyOp LessEq (VInt a) (VInt b) = Right $ VBool (a <= b)
applyOp GreaterEq (VInt a) (VInt b) = Right $ VBool (a >= b)
applyOp _ _ _ = Left "Invalid operation"