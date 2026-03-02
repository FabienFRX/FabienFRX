{-
-- EPITECH PROJECT, 2025
-- G-FUN-500-TLS-5-1-glados-4 [WSL: Debian]
-- File description:
-- Evaluator
-}

module Evaluator
    ( initialEnv
    , eval
    , evalSequence
    ) where

import AST

initialEnv :: Env
initialEnv = 
    [ ("+", VBuiltin "+")
    , ("-", VBuiltin "-")
    , ("*", VBuiltin "*")
    , ("div", VBuiltin "div")
    , ("mod", VBuiltin "mod")
    , ("eq?", VBuiltin "eq?")
    , ("<", VBuiltin "<")
    ]

eval :: Env -> AST -> Either String (Value, Env)

eval env (AInt n) = Right (VInt n, env)
eval env (ABool b) = Right (VBool b, env)

eval env (ASymbol s) = 
    case lookup s env of 
        Just v -> Right (v, env)
        Nothing -> Left $ "*** ERROR: variable " ++ s ++ " is not bound."

eval env (Define name expr) = 
    case expr of
        Lambda params body -> 
            let rec = VProcedure params body ((name, rec) : env)
            in Right (VUnit, (name, rec) : env)
        _ -> do
            (value, _) <- eval env expr
            return (VUnit, (name, value) : env)

eval env (Lambda params body) = 
    Right (VProcedure params body env, env)

eval env (If cond thenExpr elseExpr) = do
    (condVal, _) <- eval env cond
    case condVal of
        VBool True -> eval env thenExpr
        VBool False -> eval env elseExpr
        _ -> Left "*** ERROR: if condition must be a boolean"

eval env (Call funcExpr args) = do
    (funcVal, _) <- eval env funcExpr
    argVals <- mapM (fmap fst . eval env) args
    case funcVal of
        VBuiltin name -> applyBuiltin name argVals env
        VProcedure params body closureEnv -> 
            applyProcedure params body closureEnv argVals env
        _ -> Left "*** ERROR: attempting to call a non-procedure"

eval _ ast = Left ("Runtime error: unsupported operation: " ++ show ast)

applyBuiltin :: String -> [Value] -> Env -> Either String (Value, Env)
applyBuiltin "+" [VInt a, VInt b] env = Right (VInt (a + b), env)
applyBuiltin "-" [VInt a, VInt b] env = Right (VInt (a - b), env)
applyBuiltin "*" [VInt a, VInt b] env = Right (VInt (a * b), env)
applyBuiltin "div" [VInt a, VInt b] env 
    | b == 0 = Left "*** ERROR: division by zero"
    | otherwise = Right (VInt (a `div` b), env)
applyBuiltin "mod" [VInt a, VInt b] env 
    | b == 0 = Left "*** ERROR: modulo by zero"
    | otherwise = Right (VInt (a `mod` b), env)
applyBuiltin "eq?" [VInt a, VInt b] env = Right (VBool (a == b), env)
applyBuiltin "eq?" [VBool a, VBool b] env = Right (VBool (a == b), env)
applyBuiltin "<" [VInt a, VInt b] env = Right (VBool (a < b), env)
applyBuiltin name args _ = 
    Left $ "*** ERROR: invalid arguments for builtin " ++ name ++ 
           " (got " ++ show (length args) ++ " arguments)"

applyProcedure :: [String] -> AST -> Env -> [Value] -> Env -> Either String (Value, Env)
applyProcedure params body closureEnv args callerEnv
    | length params /= length args = 
        Left $ "*** ERROR: procedure expects " ++ show (length params) ++ 
               " arguments, got " ++ show (length args)
    | otherwise = do
        let newEnv = zip params args ++ closureEnv
        (result, _) <- eval newEnv body
        Right (result, callerEnv)

evalSequence :: Env -> [AST] -> Either String (Value, Env)
evalSequence env [] = Right (VUnit, env)
evalSequence env [expr] = eval env expr
evalSequence env (expr:rest) = do
    (_, newEnv) <- eval env expr
    evalSequence newEnv rest

