{-
-- EPITECH PROJECT, 2025
-- GLaDOS VM Main
-- File description:
-- Virtual Machine executable - executes bytecode
-}

module Main (main) where

import System.Environment (getArgs)
import System.Exit (exitWith, ExitCode(..))
import System.IO (hPutStrLn, stderr)
import Control.Monad (when)
import VM (runProgram)
import CompilerInterface (Bytecode(..), loadBytecode)
import Types (Value(..))

main :: IO ()
main = do
    args <- getArgs
    processArgs args

processArgs :: [String] -> IO ()
processArgs ["-h"] = printHelp
processArgs ["--help"] = printHelp
processArgs [bytecodeFile] = executeFile bytecodeFile False
processArgs ["-v", bytecodeFile] = executeFile bytecodeFile True
processArgs [] = showNoFileError
processArgs _ = showInvalidArgsError

showNoFileError :: IO ()
showNoFileError =
    hPutStrLn stderr "Error: No bytecode file specified" >>
    hPutStrLn stderr "Usage: glados-vm <bytecode-file>" >>
    exitWith (ExitFailure 84)

showInvalidArgsError :: IO ()
showInvalidArgsError =
    hPutStrLn stderr "Error: Invalid arguments" >>
    hPutStrLn stderr "Usage: glados-vm [-v] <bytecode-file>" >>
    exitWith (ExitFailure 84)

printHelp :: IO ()
printHelp = printVMHeader >> printVMUsage >> printVMExamples

printVMHeader :: IO ()
printVMHeader = putStrLn "GLaDOS Virtual Machine" >> putStrLn ""

printVMUsage :: IO ()
printVMUsage =
    putStrLn "Usage:" >>
    putStrLn "  glados-vm <bytecode-file>      " >>
    putStrLn "  glados-vm -v <bytecode-file>   " >>
    putStrLn "  glados-vm -h, --help           " >>
    putStrLn ""

printVMExamples :: IO ()
printVMExamples =
    putStrLn "Examples:" >>
    putStrLn "  glados-vm program.bc           " >>
    putStrLn "  glados-vm -v program.bc        "

executeFile :: FilePath -> Bool -> IO ()
executeFile bytecodeFile verbose = do
    content <- readFile bytecodeFile
    showLoadingInfo verbose bytecodeFile
    bytecode <- parseBytecode content
    showExecutionInfo verbose bytecode
    result <- executeProgram bytecode
    showResult verbose result

showLoadingInfo :: Bool -> FilePath -> IO ()
showLoadingInfo verbose bytecodeFile =
    when verbose $
        putStrLn (" Loading bytecode from: " ++ bytecodeFile) >>
        putStrLn ""

parseBytecode :: String -> IO Bytecode
parseBytecode content =
    case loadBytecode content of
        Left err ->
            hPutStrLn stderr ("Error: " ++ err) >>
            exitWith (ExitFailure 84)
        Right bc -> return bc

showExecutionInfo :: Bool -> Bytecode -> IO ()
showExecutionInfo verbose bytecode =
    when verbose $
        putStrLn ("Functions: " ++ show (length $ bcFunctions bytecode)) >>
        putStrLn ("Main instructions: " ++ show (length $ bcMain bytecode)) >>
        putStrLn "" >>
        putStrLn " Starting execution " >>
        putStrLn ""

executeProgram :: Bytecode -> IO Value
executeProgram bytecode =
    case runProgram (bcMain bytecode) (bcFunctions bytecode) of
        Left err ->
            hPutStrLn stderr ("Runtime error: " ++ err) >>
            exitWith (ExitFailure 84)
        Right r -> return r

showResult :: Bool -> Value -> IO ()
showResult _ result = printValue result

printValue :: Value -> IO ()
printValue (VInt n) = print n
printValue (VBool True) = putStrLn "true"
printValue (VBool False) = putStrLn "false"