{-
-- EPITECH PROJECT, 2025
-- GLaDOS AST
-- File description:
-- AST
-}

module AST (
    AST(..)
) where

data AST = 
    AInt Int
    | ABool Bool
    
    | AVar String
    | AAssign String AST              -- x = expr
    
    | AAdd AST AST                    -- x + y
    | ASub AST AST                    -- x - y
    | AMul AST AST                    -- x * y
    | ADiv AST AST                    -- x / y
    | AEq AST AST                     -- x == y
    | ANotEq AST AST                  -- x != y
    | ALess AST AST                   -- x < y
    | AGreater AST AST                -- x > y
    | ALessEq AST AST                 -- x <= y
    | AGreaterEq AST AST              -- x >= y
    
    | AIf AST AST AST                 -- if (cond) then else
    
    | AFuncDef String [String] AST    -- fn name(params) { body }
    | ACall String [AST]              -- name(args)
    | AReturn AST                     -- return expr
    
    | ASeq [AST]                      -- stmt1; stmt2; ...
    
    deriving (Show, Eq)