{-
-- EPITECH PROJECT, 2025
-- GLaDOS VM Types
-- File description:
-- Types for VM 
-}

module Types (
    Value(..),
    Op(..),
    Instruction(..),
    VMState(..),
    CallFrame(..),
    initVM
) where

import qualified Data.Map as Map

data Value = 
    VInt Int
    | VBool Bool
    deriving (Show, Eq, Read)

data Op =
    Add | Sub | Mul | Div
    | Eq | NotEq | Less | Greater | LessEq | GreaterEq
    deriving (Show, Eq, Read)

data Instruction =
    -- Stack
    Push Value
    | Pop
    
    | LoadVar String
    | StoreVar String
    
    | BinOp Op
    
    | JumpIfFalse Int
    | Jump Int
    
    | CallFunc String Int
    | Return
    | LoadArg Int
    
    deriving (Show, Eq, Read)

data VMState = VMState
    { stack :: [Value]
    , vars :: Map.Map String Value
    , functions :: Map.Map String ([String], [Instruction])  -- (params, body)
    , pc :: Int                -- Program counter
    } deriving (Show)

data CallFrame = CallFrame
    { returnPC :: Int
    , localVars :: Map.Map String Value
    } deriving (Show)

initVM :: VMState
initVM = VMState
    { stack = []
    , vars = Map.empty
    , functions = Map.empty
    , pc = 0
    }