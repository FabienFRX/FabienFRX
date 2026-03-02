{-
-- EPITECH PROJECT, 2025
-- GLaDOS Compiler
-- File description:
-- Compiler AST → Bytecode
-}

module Compiler (compile, extractFunctions, compileAST) where

import AST
import Types
import qualified Data.Map as Map

-- Compile AST to Bytecode instructions and function map
compile :: [AST] -> Either String ([Instruction], Map.Map String ([String], [Instruction]))
compile asts = do
    let (funcs, topLevel) = extractFunctions asts
    validateTopLevel topLevel
    funcMap <- compileFunctions funcs
    validateMainFunction funcMap
    let mainCode = [CallFunc "main" 0, Return]
    return (mainCode, funcMap)

validateTopLevel :: [AST] -> Either String ()
validateTopLevel topLevel =
    if not (null topLevel)
        then Left "Top-level code not allowed. Use functions only."
        else return ()

validateMainFunction :: Map.Map String ([String], [Instruction]) -> Either String ()
validateMainFunction funcMap =
    case Map.lookup "main" funcMap of
        Nothing -> Left "Entry point 'main()' function not found"
        Just (params, _) ->
            if not (null params)
                then Left "main() function must have no parameters"
                else return ()

extractFunctions :: [AST] -> ([AST], [AST])
extractFunctions = foldr separate ([], [])
  where
    separate f@(AFuncDef _ _ _) (funcs, main) = (f : funcs, main)
    separate stmt (funcs, main) = (funcs, stmt : main)

compileFunctions :: [AST] -> Either String (Map.Map String ([String], [Instruction]))
compileFunctions funcs = do
    compiled <- mapM compileFunction funcs
    return $ Map.fromList compiled

compileFunction :: AST -> Either String (String, ([String], [Instruction]))
compileFunction (AFuncDef name params body) = do
    bodyCode <- compileAST body
    let codeWithReturn = bodyCode ++ [Return]
    return (name, (params, codeWithReturn))
compileFunction _ = Left "Not a function definition"

compileStatements :: [AST] -> Either String [Instruction]
compileStatements stmts = do
    codes <- mapM compileAST stmts
    return $ concat codes

compileAST :: AST -> Either String [Instruction]

-- Literals
compileAST (AInt n) = Right [Push (VInt n)]
compileAST (ABool b) = Right [Push (VBool b)]

-- Variables
compileAST (AVar name) = Right [LoadVar name]
compileAST (AAssign name expr) = do
    code <- compileAST expr
    return $ code ++ [StoreVar name]

-- Binary Operations
compileAST (AAdd l r) = compileBinOp Add l r
compileAST (ASub l r) = compileBinOp Sub l r
compileAST (AMul l r) = compileBinOp Mul l r
compileAST (ADiv l r) = compileBinOp Div l r
compileAST (AEq l r) = compileBinOp Eq l r
compileAST (ANotEq l r) = compileBinOp NotEq l r
compileAST (ALess l r) = compileBinOp Less l r
compileAST (AGreater l r) = compileBinOp Greater l r
compileAST (ALessEq l r) = compileBinOp LessEq l r
compileAST (AGreaterEq l r) = compileBinOp GreaterEq l r

-- If-else
compileAST (AIf cond thenBranch elseBranch) = do
    condCode <- compileAST cond
    thenCode <- compileAST thenBranch
    elseCode <- compileAST elseBranch
    return $ compileIfElse condCode thenCode elseCode
compileAST (ACall name args) = do
    argCodes <- mapM compileAST args
    return $ concat argCodes ++ [CallFunc name (length args)]
compileAST (AReturn expr) = do
    code <- compileAST expr
    return $ code ++ [Return]
compileAST (ASeq []) = Right []
compileAST (ASeq stmts) = compileStatements stmts
compileAST (AFuncDef _ _ _) = Left "Function definitions not compiled directly"

compileIfElse :: [Instruction] -> [Instruction] -> [Instruction] -> [Instruction]
compileIfElse condCode thenCode elseCode =
    let thenLen = length thenCode
        elseLen = length elseCode
    in condCode ++
       [JumpIfFalse (thenLen + 1)] ++
       thenCode ++
       [Jump elseLen] ++
       elseCode

compileBinOp :: Op -> AST -> AST -> Either String [Instruction]
compileBinOp op left right = do
    leftCode <- compileAST left
    rightCode <- compileAST right
    return $ leftCode ++ rightCode ++ [BinOp op]