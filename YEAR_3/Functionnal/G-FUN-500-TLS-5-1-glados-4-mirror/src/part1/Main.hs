{-
-- EPITECH PROJECT, 2025
-- my_pandoc
-- File description:
-- Main
-}

module Main (main) where

import Parser
import AST
import Evaluator
import System.Exit (exitWith, ExitCode(..))
import System.IO (hPutStrLn, stderr, hFlush, stdout, isEOF, stdin, hIsTerminalDevice)
import System.Environment (getArgs)
import Control.Monad (when, unless)
import CParser
import Compiler

main :: IO ()
main = getArgs >>= handleArgs

handleArgs :: [String] -> IO ()
handleArgs [] = handleNoArgs
handleArgs ["-i"] = runREPL
handleArgs ["--dis", filename] = runCDis filename
handleArgs [filename] = runFile filename
handleArgs _ = showUsageAndExit

handleNoArgs :: IO ()
handleNoArgs = do
    isTTY <- hIsTerminalDevice stdin
    if isTTY then runREPL else runStdin

showUsageAndExit :: IO ()
showUsageAndExit = hPutStrLn stderr usageMsg >> exitWith (ExitFailure 84)
  where
    usageMsg = "Usage: glados [filename] | glados -i | glados --dis <file>"

runFile :: FilePath -> IO ()
runFile filename = do
    input <- readFile filename
    case runInterpreter input of
        Left err -> hPutStrLn stderr err >> exitWith (ExitFailure 84)
        Right result -> putStr result

runStdin :: IO ()
runStdin = do
    input <- getContents
    case runInterpreter input of
        Left err -> hPutStrLn stderr err >> exitWith (ExitFailure 84)
        Right result -> putStr result

runREPL :: IO ()
runREPL = putStrLn "GLaDOS LISP Interpreter v1.0" >>
    putStrLn "Type expressions to evaluate, or Ctrl+D to exit" >>
    replLoop initialEnv

replLoop :: Env -> IO ()
replLoop env = do
    putStr "> "
    hFlush stdout
    eof <- isEOF
    unless eof $ processInput env

processInput :: Env -> IO ()
processInput env = do
    input <- getLine
    unless (null input) $ handleInput env input
    when (null input) $ replLoop env

handleInput :: Env -> String -> IO ()
handleInput env input = case runInterpreterWithEnv env input of
    Left err -> putStrLn err >> replLoop env
    Right (result, newEnv) -> putStr result >> replLoop newEnv

runInterpreter :: String -> Either String String
runInterpreter input = fst <$> parseAndEval input initialEnv

parseAndEval :: String -> Env -> Either String (String, Env)
parseAndEval input env = do
    sexprs <- parseInput input
    asts <- mapM sexprToAST sexprs
    (results, newEnv) <- evalAll env asts
    return (formatResults results, newEnv)

parseInput :: String -> Either String [SExpr]
parseInput input = case parseProgram input of
    Left err -> Left $ "*** PARSE ERROR: " ++ show err
    Right s -> Right s

formatResults :: [Value] -> String
formatResults results = unlines $ map valueToString filteredResults
  where
    filteredResults = filter (not . isUnit) results

runInterpreterWithEnv :: Env -> String -> Either String (String, Env)
runInterpreterWithEnv = flip parseAndEval

evalAll :: Env -> [AST] -> Either String ([Value], Env)
evalAll env [] = Right ([], env)
evalAll env (ast:rest) = do
    (value, newEnv) <- eval env ast
    (values, finalEnv) <- evalAll newEnv rest
    Right (value : values, finalEnv)

isUnit :: Value -> Bool
isUnit VUnit = True
isUnit _ = False

runCDis :: FilePath -> IO ()
runCDis filename = do
    input <- readFile filename
    case parseCProgram input of
        Left err -> hPutStrLn stderr ("*** PARSE ERROR: " ++ err) >> exitWith (ExitFailure 84)
        Right asts -> case compileProgram asts of
            Left e -> hPutStrLn stderr ("*** COMPILE ERROR: " ++ show e) >> exitWith (ExitFailure 84)
            Right code -> putStr (disassemble code)
