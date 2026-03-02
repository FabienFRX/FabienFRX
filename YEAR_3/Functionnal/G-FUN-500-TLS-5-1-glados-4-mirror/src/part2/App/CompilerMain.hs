{-
-- EPITECH PROJECT, 2025
-- GLaDOS Compiler Main
-- File description:
-- Compiler executable - compiles source code to bytecode
-}

module Main (main) where

import System.Environment (getArgs)
import System.Exit (exitWith, ExitCode(..))
import System.IO (hPutStrLn, stderr)
import System.FilePath (takeExtension)
import AST
import Types (Instruction)
import Parser (parseProgram)
import Compiler (compile)
import CompilerInterface (Bytecode(..), saveBytecode, printbytecode)
import qualified Data.Map as Map

main :: IO ()
main = do
    args <- getArgs
    processCompilerArgs args

processCompilerArgs :: [String] -> IO ()
processCompilerArgs ["-h"] = printHelp
processCompilerArgs ["--help"] = printHelp
processCompilerArgs ["-o", outFile, inFile] = compileFile inFile (Just outFile)
processCompilerArgs [inFile] = compileFile inFile Nothing
processCompilerArgs [] = showNoInputError
processCompilerArgs _ = showInvalidCompilerArgsError

showNoInputError :: IO ()
showNoInputError =
    hPutStrLn stderr "Error: No input file specified" >>
    hPutStrLn stderr "Usage: glados-compiler [-o output] <input-file>" >>
    exitWith (ExitFailure 84)

showInvalidCompilerArgsError :: IO ()
showInvalidCompilerArgsError =
    hPutStrLn stderr "Error: Invalid arguments" >>
    hPutStrLn stderr "Usage: glados-compiler [-o output] <input-file>" >>
    exitWith (ExitFailure 84)

printHelp :: IO ()
printHelp = printCompilerHeader >> printCompilerUsage >> printCompilerExamples

printCompilerHeader :: IO ()
printCompilerHeader = putStrLn "GLaDOS Compiler" >> putStrLn ""

printCompilerUsage :: IO ()
printCompilerUsage =
    putStrLn "Usage:" >>
    putStrLn "  glados-compiler <input-file>              " >>
    putStrLn "  glados-compiler -o <out> <input>          " >>
    putStrLn "  glados-compiler -h, --help                " >>
    putStrLn ""

printCompilerExamples :: IO ()
printCompilerExamples =
    putStrLn "Examples:" >>
    putStrLn "  glados-compiler program.gld               " >>
    putStrLn "  glados-compiler -o program.bc program.gld "

compileFile :: FilePath -> Maybe FilePath -> IO ()
compileFile inFile mOutFile = do
    validateFileExtension inFile
    sourceCode <- readFile inFile
    ast <- parseSourceCode sourceCode
    (mainCode, funcs) <- compileAST ast
    let bytecode = createBytecode funcs mainCode
    outputBytecode mOutFile bytecode

validateFileExtension :: FilePath -> IO ()
validateFileExtension inFile =
    if takeExtension inFile /= ".gld"
        then hPutStrLn stderr "Error: Input file must have .gld extension" >>
             exitWith (ExitFailure 84)
        else return ()

parseSourceCode :: String -> IO [AST]
parseSourceCode sourceCode =
    case parseProgram sourceCode of
        Left err ->
            hPutStrLn stderr ("Parse error: " ++ err) >>
            exitWith (ExitFailure 84)
        Right a -> return a

compileAST :: [AST] -> IO ([Instruction], Map.Map String ([String], [Instruction]))
compileAST ast =
    case compile ast of
        Left err ->
            hPutStrLn stderr ("Compilation error: " ++ err) >>
            exitWith (ExitFailure 84)
        Right c -> return c

createBytecode :: Map.Map String ([String], [Instruction]) -> [Instruction] -> Bytecode
createBytecode funcs mainCode = Bytecode
    { bcFunctions = funcs
    , bcMain = mainCode
    }

outputBytecode :: Maybe FilePath -> Bytecode -> IO ()
outputBytecode Nothing bytecode = putStrLn $ printbytecode bytecode
outputBytecode (Just outFile) bytecode =
    writeFile outFile (saveBytecode bytecode) >>
    putStrLn "[OK] Compilation successful!" >>
    putStrLn ("[OK] Bytecode written to: " ++ outFile)