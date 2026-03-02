{-
-- EPITECH PROJECT, 2025
-- B-PDG-300-TLS-3-1-PDGD02-fabien.fraixanet
-- File description:
-- DoOp
-}

import Data.Char
import System.Environment (getArgs)
import System.Exit (exitWith, ExitCode(ExitFailure))
import System.IO (hPutStrLn, stderr)

myElem :: Eq a => a -> [a] -> Bool
myElem _[] = False
myElem y (x:xs) = if y == x then True else myElem y xs

safeDiv :: Int -> Int -> Maybe Int
safeDiv x 0 = Nothing
safeDiv x y = Just (x `div` y)

safeNth :: [a] -> Int -> Maybe a
safeNth [] _ = Nothing
safeNth (x : xs) 0 = Just x
safeNth (_ : xs) n = safeNth xs (n - 1)

safeSucc :: Maybe Int -> Maybe Int
safeSucc = fmap succ

myLookup :: Eq a => a -> [(a, b)] -> Maybe b
myLookup _ [] = Nothing
myLookup e ((x, y) : ys) = if e == x then Just y else myLookup e ys

maybeDo :: (a -> b -> c) -> Maybe a -> Maybe b -> Maybe c
maybeDo f ma mb = do
    x <- ma
    y <- mb
    return (f x y)

isSign :: Char -> Bool
isSign x = x == '-' || x == '+'

readInt :: [Char] -> Maybe Int
readInt "" = Nothing
readInt (x:xs)
    | (isSign x) && all isDigit xs = Just (read (x:xs))
    | all isDigit (x:xs) = Just (read (x:xs))
    | otherwise = Nothing

getLineLength :: IO Int
getLineLength = do 
    s <- getLine
    return (length s)

printAndGetLength :: String -> IO Int
printAndGetLength s = putStrLn s >> return (length s)

printBox :: Int -> IO ()
printBox n
    | n <= 0 = return ()
    | n <= 1 = putStrLn "++"
    | otherwise = putStrLn ('+' : replicate (2 * n - 2) '-' ++ "+") >>
               mapM_ (const
                       (putStrLn
                         ('|' : replicate (2 * n - 2) ' ' ++ "|")))
                     [1..(n - 2)] >>
               putStrLn ('+' : replicate (2 * n - 2) '-' ++ "+")

concatLines :: Int -> IO String
concatLines n
    | n <= 0 = return ""
    | otherwise = do
        line <- getLine
        rest <- concatLines (n - 1)
        return (line ++ rest)

getInt :: IO (Maybe Int)
getInt = getLine >>= \x -> return (readInt x)

doop :: Int -> String -> Int -> Int
doop x "+" y = x + y
doop x "-" y = x - y
doop x "*" y = x * y
doop x "/" y = if y == 0 then 84 else x `div` y
doop x "%" y = if y == 0 then 84 else x `mod` y
doop _ _ _ = 84

main :: IO ()
main = do
    args <- getArgs
    case args of
        [a, op, b] -> case (readInt a, readInt b) of
            (Just x, Just y) -> if doop x op y == 84 
                then exitWith (ExitFailure 84) 
                else putStrLn (show (doop x op y))
            _ -> exitError
        _ -> exitError
  where exitError = exitWith (ExitFailure 84)