module SExpr
  ( SExpr(..)
  , getSymbol
  , getInteger
  , getList
  , printTree
  ) where

import           Data.List (intercalate)

-- SExpr : représentation brute d'un programme Lisp/Scheme
data SExpr
  = SInt Integer
  | SSymbol String
  | SList [SExpr]
  deriving (Show, Eq)

-- renvoie le symbole si c'en est un
getSymbol :: SExpr -> Maybe String
getSymbol (SSymbol s) = Just s
getSymbol _           = Nothing

-- renvoie l'entier si c'en est un
getInteger :: SExpr -> Maybe Integer
getInteger (SInt n) = Just n
getInteger _        = Nothing

-- renvoie la liste si c'en est une
getList :: SExpr -> Maybe [SExpr]
getList (SList xs) = Just xs
getList _          = Nothing

-- pretty-print simple pour comprendre la structure (utile pour debug)
printTree :: SExpr -> String
printTree (SInt n) = "a Number " ++ show n
printTree (SSymbol s) = "a Symbol " ++ show s
printTree (SList xs) =
  "a List with "
    ++ show (length xs)
    ++ " elements: ["
    ++ intercalate ", " (map printTree xs)
    ++ "]"
