module AST
  ( AST(..)
  , sexprToAST
  ) where

import           SExpr

-- AST : plus sémantique que SExpr
data AST
  = ADefine String AST -- (define name expr)
  | ASymbol String -- symbole (nom de variable ou de fonction)
  | ANumber Integer -- nombres entiers
  | ABoolean Bool -- booleans #t / #f
  | ACall AST [AST] -- appel (operator arg1 arg2 ...)
  deriving (Show, Eq)

-- Conversion partielle SExpr -> AST
sexprToAST :: SExpr -> Maybe AST
sexprToAST (SInt n) = Just (ANumber n)
sexprToAST (SSymbol "#t") = Just (ABoolean True)
sexprToAST (SSymbol "#f") = Just (ABoolean False)
sexprToAST (SSymbol s) = Just (ASymbol s)
sexprToAST (SList [SSymbol "define", SSymbol var, val]) =
  case sexprToAST val of
    Just v  -> Just (ADefine var v)
    Nothing -> Nothing
-- appel général : (f arg1 arg2 ...)
sexprToAST (SList (func:args)) = do
  f' <- sexprToAST func
  as <- mapM sexprToAST args
  Just (ACall f' as)
sexprToAST _ = Nothing
