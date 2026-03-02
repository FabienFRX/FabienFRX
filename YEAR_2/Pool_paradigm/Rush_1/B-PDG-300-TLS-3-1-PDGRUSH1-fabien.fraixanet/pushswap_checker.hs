{-
-- EPITECH PROJECT, 2025
-- GitHub
-- File description:
-- pushswap_checker
-}

import System.Environment (getArgs)
import System.IO (getLine)
import System.Exit (exitWith, ExitCode(..))
import Data.List (sort)
import Text.Read (readMaybe)

type Stack = [Int]

sa :: (Stack, Stack) -> (Stack, Stack)
sa (x:y:xs, b) = (y:x:xs, b)
sa (a, b) = (a, b)

sb :: (Stack, Stack) -> (Stack, Stack)
sb (a, x:y:xs) = (a, y:x:xs)
sb (a, b) = (a, b)

sc :: (Stack, Stack) -> (Stack, Stack)
sc (a, b) = let rt = sa (a, b) in sb rt

pa :: (Stack, Stack) -> (Stack, Stack)
pa (a, b:xs) = (b:a, xs)
pa (a, []) = (a, [])

pb :: (Stack, Stack) -> (Stack, Stack)
pb ([], b) = ([], b)
pb ([x], b) = ([], x:b)
pb (x:xs, b) = (xs, x:b)

ra :: (Stack, Stack) -> (Stack, Stack)
ra ([], b) = ([], b)
ra ([x], b) = ([x], b)
ra (x:xs, b) = (xs ++ [x], b)

rb :: (Stack, Stack) -> (Stack, Stack)
rb (a, []) = (a, [])
rb (a, [x]) = (a, [x])
rb (a, x:xs) = (a, xs ++ [x])

rr :: (Stack, Stack) -> (Stack, Stack)
rr (a, b) = let (a', b') = ra (a, b) in rb (a', b')

rra :: (Stack, Stack) -> (Stack, Stack)
rra (a, b) = (last a : init a, b)

rrb :: (Stack, Stack) -> (Stack, Stack)
rrb (a, b) = (a, last b : init b)

rrr :: (Stack, Stack) -> (Stack, Stack)
rrr (a, b) = let (a', b') = rra (a, b) in rrb (a', b')

isSorted :: [Int] -> Bool
isSorted [] = True
isSorted [_] = True
isSorted (x:y:xs) = x <= y && isSorted (y:xs)

handleResult :: Maybe (Stack, Stack) -> IO ()
handleResult (Just (finalA, finalB)) =
    let result = if isSorted finalA && null finalB then 0 else 1
    in putStrLn (if result == 0 then "OK" else "KO") >>
       exitWith (if result == 0 then ExitSuccess else ExitFailure 84)
handleResult Nothing =
    putStrLn "KO: Invalid operation" >>
    exitWith (ExitFailure 84)

parseArgs :: IO ([Int], [String])
parseArgs = do
    args <- getArgs
    input <- getLine
    let stackA = mapM readMaybe args :: Maybe [Int]
        operations = words input
    case stackA of
        Just stack -> return (stack, operations)
        Nothing -> 
            putStrLn "KO: Invalid number format" >> exitWith (ExitFailure 84)

processOperation1 :: (Stack, Stack) -> String -> (Stack, Stack)
processOperation1 stacks op = case op of
    "sa" -> sa stacks
    "sb" -> sb stacks
    "sc" -> sc stacks
    "pa" -> pa stacks
    "pb" -> pb stacks
    _    -> stacks
 
processOperation2 :: (Stack, Stack) -> String -> (Stack, Stack)
processOperation2 stacks op = case op of
    "ra" -> ra stacks
    "rb" -> rb stacks
    "rr" -> rr stacks
    "rra" -> rra stacks
    "rrb" -> rrb stacks
    "rrr" -> rrr stacks
    _    -> stacks
 
processOperations :: Stack -> Stack -> [String] -> IO (Maybe (Stack, Stack))
processOperations stackA stackB [] = return $ Just (stackA, stackB)
processOperations stackA stackB (op:ops) =
    let stacks1 = processOperation1 (stackA, stackB) op
        stacks2 = processOperation2 stacks1 op
    in uncurry processOperations stacks2 ops

main :: IO ()
main = do
    (stackA, operations) <- parseArgs
    handleInput stackA operations

handleInput :: Stack -> [String] -> IO ()
handleInput stackA operations
    | null stackA = handleNoInput
    | null operations = handleNoOperations stackA
    | otherwise = handleOperations stackA operations

handleNoInput :: IO ()
handleNoInput =
    putStrLn "KO: No input numbers provided" >>
    exitWith (ExitFailure 84)

handleNoOperations :: Stack -> IO ()
handleNoOperations stackA
    | isSorted stackA = putStrLn "OK" >> exitWith ExitSuccess
    | otherwise = 
        putStrLn "KO: No operations provided" >> exitWith (ExitFailure 84)

handleOperations :: Stack -> [String] -> IO ()
handleOperations stackA operations = do
    let stackB = []
    result <- processOperations stackA stackB operations
    handleResult result
