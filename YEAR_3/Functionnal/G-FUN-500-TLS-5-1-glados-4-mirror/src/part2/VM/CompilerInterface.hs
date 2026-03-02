{-
-- EPITECH PROJECT, 2025
-- GLaDOS Compiler Interface
-- File description:
-- Bytecode Serialization/Deserialization
-}

module CompilerInterface
    ( Bytecode(..)
    , saveBytecode
    , loadBytecode
    , printbytecode
    ) where

import Types
import qualified Data.Map as Map

data Bytecode = Bytecode
    { bcFunctions :: Map.Map String ([String], [Instruction])
    , bcMain :: [Instruction]
    } deriving (Show, Read)

-- Save bytecode to string
saveBytecode :: Bytecode -> String
saveBytecode = show

-- Load bytecode from string
loadBytecode :: String -> Either String Bytecode
loadBytecode content = 
    case reads content of
        [(bc, _)] -> Right bc
        _ -> Left "Invalid bytecode format"

-- Print bytecode
printbytecode :: Bytecode -> String
printbytecode bc =
    "GLaDOS Bytecode\n\n" ++
    serializeFunctions (Map.toList $ bcFunctions bc) ++
    "\nMain Program\n" ++
    serializeInstructions (bcMain bc)

serializeFunctions :: [(String, ([String], [Instruction]))] -> String
serializeFunctions [] = "(no functions)"
serializeFunctions funcs = concatMap formatFunc funcs
  where
    formatFunc (name, (params, body)) =
        name ++ "(" ++ unwords params ++ ")\n" ++
        serializeInstructions body ++ "\n"

serializeInstructions :: [Instruction] -> String
serializeInstructions instrs =
    unlines $ zipWith formatInstr ([0..] :: [Int]) instrs
  where
    formatInstr idx instr = padLeft 4 (show idx) ++ ": " ++ show instr

padLeft :: Int -> String -> String
padLeft n s = replicate (max 0 (n - length s)) ' ' ++ s