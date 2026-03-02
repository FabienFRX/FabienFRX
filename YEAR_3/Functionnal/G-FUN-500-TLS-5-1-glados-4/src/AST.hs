{-
-- EPITECH PROJECT, 2025
-- G-FUN-500-TLS-5-1-glados-4 [WSL: Debian]
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

-- Symbolic Expressions
data SExpr = 
    SInt Integer
    | SBool Bool
    | SSymbol String
    | SList [SExpr]
    deriving (Show, Eq)

-- Abstract Syntax Tree
data AST = 
    AInt Integer
    | ABool Bool
    | ASymbol String
    | Define String AST
    | Lambda [String] AST
    | If AST AST AST
    | Call AST [AST]
    deriving (Show, Eq)

-- Values that can result from evaluation
data Value = 
    VInt Integer
    | VBool Bool
    | VProcedure [String] AST Env
    | VBuiltin String
    | VUnit  -- For expressions that don't return a value
    deriving (Show, Eq)

-- Environment for storing bindings
type Env = [(String, Value)]

-- Convert S-expression to AST
sexprToAST :: SExpr -> Either String AST
sexprToAST (SInt n) = Right (AInt n)
sexprToAST (SBool b) = Right (ABool b)
sexprToAST (SSymbol s) = Right (ASymbol s)

-- Handle 'define' special form
sexprToAST (SList [SSymbol "define", SSymbol name, expr]) = do
    ast <- sexprToAST expr
    Right (Define name ast)

-- Handle 'define' for functions
sexprToAST (SList (SSymbol "define" : 
                   SList (SSymbol name : params) : 
                   body : [])) = do
    paramNames <- mapM getSymbol params
    bodyAST <- sexprToAST body
    Right (Define name (Lambda paramNames bodyAST))

-- Handle 'lambda' special form
sexprToAST (SList [SSymbol "lambda", SList params, body]) = do
    paramNames <- mapM getSymbol params
    bodyAST <- sexprToAST body
    Right (Lambda paramNames bodyAST)

-- Handle 'if' special form
sexprToAST (SList [SSymbol "if", cond, thenExpr, elseExpr]) = do
    condAST <- sexprToAST cond
    thenAST <- sexprToAST thenExpr
    elseAST <- sexprToAST elseExpr
    Right (If condAST thenAST elseAST)

-- Handle function calls (including built-in operators)
sexprToAST (SList (func : args)) = do
    funcAST <- sexprToAST func
    argASTs <- mapM sexprToAST args
    Right (Call funcAST argASTs)

-- Empty list
sexprToAST (SList []) = Left "Empty list is not a valid expression"

getSymbol :: SExpr -> Either String String
getSymbol (SSymbol s) = Right s
getSymbol _ = Left "Expected symbol in parameter list"

-- Convert Value back to displayable string
valueToString :: Value -> String
valueToString (VInt n) = show n
valueToString (VBool True) = "#t"
valueToString (VBool False) = "#f"
valueToString (VProcedure _ _ _) = "#<procedure>"
valueToString (VBuiltin name) = "#<builtin:" ++ name ++ ">"
valueToString VUnit = "#<unit>"