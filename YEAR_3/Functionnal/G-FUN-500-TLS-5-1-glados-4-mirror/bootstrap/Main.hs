{-
-- EPITECH PROJECT, 2025
-- part2-bs
-- File description:
-- Main
-}

data Value
  = VInt Int
  | VBool Bool
  | VOp Op
  | VFunc Int Insts
  deriving (Show, Eq)

type Stack = [Value]
type Insts = [Instruction]
type Arg = [Value]
type Env = [(String, Value)]

data Op
  = Add | Sub | Mul | Div
  | Eq | Less
  deriving (Show, Eq)

data Instruction
  = Push Value
  | JumpIfFalse Int
  | Call
  | Ret
  | PushArg Int
  | PushEnv String
  | Dup    -- duplique le premier
  | Pop    -- retire le premier
  | Swap   -- échager les 2 premiers
  deriving (Show, Eq)

getInt :: Value -> Either String Int
getInt (VInt n) = Right n
getInt v        = Left $ "Expected Int, got: " ++ show v

getBool :: Value -> Either String Bool
getBool (VBool b) = Right b
getBool v         = Left $ "Expected Bool, got: " ++ show v

exec :: Env -> Arg -> Insts -> Stack -> Either String Value
exec _ _ [] stack = 
    case stack of
        (v:_) -> Right v
        []    -> Left "Empty stack at end of execution"

exec env args (inst:irest) stack =
    case inst of
        Push v -> exec env args irest (v:stack)

        Call -> case stack of
            (VOp op : first : second : srest) -> 
                case applyOp op second first of
                    Right result -> exec env args irest (result : srest)
                    Left err -> Left err
            
            (VFunc nbArgs funcInsts : srest) -> 
                if nbArgs > length srest
                then Left ("Not Enough argument (needed " ++ show nbArgs ++ ")")
                else
                    case exec env funcArgsRev funcInsts [] of
                        Right result -> exec env args irest (result : remaining)
                        Left err -> Left err
                      where
                        (funcArgs, remaining) = splitAt nbArgs srest
                        funcArgsRev = reverse funcArgs
            
            _ -> Left "Invalid call: expected operator or function on stack"
        
        JumpIfFalse nb -> case stack of
            (cond:srest) -> 
                case getBool cond of
                    Right False -> exec env args (drop nb irest) srest
                    Right True  -> exec env args irest srest
                    Left err    -> Left err
            [] -> Left "Empty stack for JumpIfFalse"

        Ret -> case stack of
            (v:_) -> Right v
            []    -> Left "Empty stack on Ret"

        PushArg idx ->
            if idx >= 0 && idx < length args
            then exec env args irest (args !! idx : stack)
            else Left $ "Invalid argument index: " ++ show idx

        PushEnv name ->
            case lookup name env of
                Just val -> exec env args irest (val : stack)
                Nothing  -> Left $ "Undefined variable: " ++ name
        Dup ->
            case stack of
                (value:srest) -> exec env args irest (value : value : srest)
                [] -> Left "Empty stack : Can't duplicate"
        
        Pop ->
            case stack of
                (value:srest) -> exec env args irest (srest)
                [] Left "Empty stack : Nothing to Pop"
        
        Swap ->
            case stack of
                (first:second:srest) -> exec env args irest (second:first:srest)
                [_] -> Left "Stack length = 1: Impossible to swap only one element"
                [] -> Left "Stack length = 0: Impossible to swap nothing"

        

applyOp :: Op -> Value -> Value -> Either String Value
applyOp Add v1 v2 = do
    n1 <- getInt v1
    n2 <- getInt v2
    return $ VInt (n1 + n2)
applyOp Sub v1 v2 = do
    n1 <- getInt v1
    n2 <- getInt v2
    return $ VInt (n1 - n2)
applyOp Mul v1 v2 = do
    n1 <- getInt v1
    n2 <- getInt v2
    return $ VInt (n1 * n2)
applyOp Div v1 v2 = do
    n1 <- getInt v1
    n2 <- getInt v2
    if n2 == 0
    then Left "Division by zero"
    else return $ VInt (n1 `div` n2)
applyOp Eq v1 v2 = do
    n1 <- getInt v1
    n2 <- getInt v2
    return $ VBool (n1 == n2)
applyOp Less v1 v2 = do
    n1 <- getInt v1
    n2 <- getInt v2
    return $ VBool (n1 < n2)