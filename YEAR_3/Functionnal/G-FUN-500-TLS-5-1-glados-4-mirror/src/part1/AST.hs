{-
-- EPITECH PROJECT, 2025
-- G-FUN-500-TLS-5-1-glados-4 ANDI
-- File description:
-- AST
-}

module AST
    ( SExpr(..)
    , AST(..)
    , Value(..)
    , Env
    , sexprToAST
    , valueToString
    ) where

data SExpr =
    SInt Integer
  | SBool Bool
  | SSymbol String
  | SList [SExpr]
  deriving (Show, Eq)

data AST =
    AInt Integer
  | ABool Bool
  | ASymbol String
  | Define String AST
  | Assignment String AST
  | Lambda [String] AST
  | If AST AST AST
  | IfElse AST AST AST
  | Call AST [AST]
  | Return AST
  | Block [AST]
  | BinaryOp String AST AST
  deriving (Show, Eq)

data Value =
    VInt Integer
  | VBool Bool
  | VProcedure [String] AST Env
  | VBuiltin String
  | VUnit
  deriving (Show, Eq)

type Env = [(String, Value)]

sexprToAST :: SExpr -> Either String AST
sexprToAST (SInt n) = Right (AInt n)
sexprToAST (SBool b) = Right (ABool b)
sexprToAST (SSymbol s) = Right (ASymbol s)
sexprToAST (SList [SSymbol "define", SSymbol name, expr]) = do
    ast <- sexprToAST expr
    Right (Define name ast)
sexprToAST (SList [SSymbol "define", SList (SSymbol name : params), body]) = do
    paramNames <- mapM getSymbol params
    bodyAST <- sexprToAST body
    Right (Define name (Lambda paramNames bodyAST))
sexprToAST (SList [SSymbol "lambda", SList params, body]) = do
    paramNames <- mapM getSymbol params
    bodyAST <- sexprToAST body
    Right (Lambda paramNames bodyAST)
sexprToAST (SList [SSymbol "if", cond, thenExpr, elseExpr]) = do
    condAST <- sexprToAST cond
    thenAST <- sexprToAST thenExpr
    elseAST <- sexprToAST elseExpr
    Right (If condAST thenAST elseAST)
sexprToAST (SList (func : args)) = do
    funcAST <- sexprToAST func
    argASTs <- mapM sexprToAST args
    Right (Call funcAST argASTs)
sexprToAST (SList []) = Left "Empty list is not a valid expression"

getSymbol :: SExpr -> Either String String
getSymbol (SSymbol s) = Right s
getSymbol _ = Left "Expected symbol in parameter list"

valueToString :: Value -> String
valueToString (VInt n) = show n
valueToString (VBool True) = "#t"
valueToString (VBool False) = "#f"
valueToString (VProcedure {}) = "#<procedure>"
valueToString (VBuiltin name) = "#<builtin:" ++ name ++ ">"
valueToString VUnit = "#<unit>"
