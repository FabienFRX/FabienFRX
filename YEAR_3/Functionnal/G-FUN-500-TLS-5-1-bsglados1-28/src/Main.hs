module Main where

import           AST
import           Eval
import           SExpr

-- petits exemples écrits en SExpr (on évite le parsing pour l'instant)
defineX :: SExpr
defineX = SList [SSymbol "define", SSymbol "x", SInt 5]

callPlus :: SExpr
callPlus = SList [SSymbol "+", SSymbol "x", SInt 3]

callFactExample :: SExpr
callFactExample =
  SList [SSymbol "define", SSymbol "y", SList [SSymbol "+", SInt 10, SInt 32]]

main :: IO ()
main = do
  putStrLn "=== GLaDOS Bootstrap demo ==="
    -- convert et eval define x
  case sexprToAST defineX of
    Nothing -> putStrLn "sexpr -> ast (defineX) failed"
    Just ast -> do
      putStrLn $ "AST defineX : " ++ show ast
      case evalTop emptyEnv ast of
        Left err -> putStrLn $ "Error during evalTop: " ++ err
        Right (val, env') -> do
          putStrLn $ "Defined x => " ++ prettyPrintValue val
            -- maintenant évaluer (+ x 3)
          case sexprToAST callPlus of
            Nothing -> putStrLn "sexpr -> ast (callPlus) failed"
            Just ast2 ->ki
              case evalTop env' ast2 of
                Left err2 -> putStrLn $ "Error during eval: " ++ err2
                Right (val2, _) ->
                  putStrLn $ "(+ x 3) => " ++ prettyPrintValue val2
    -- un autre exemple
  case sexprToAST callFactExample of
    Just ast3 ->
      case evalTop emptyEnv ast3 of
        Right (v, _) -> putStrLn $ "example (+ 10 32) => " ++ prettyPrintValue v
        Left e       -> putStrLn $ "err: " ++ e
    Nothing -> putStrLn "sexpr->ast failed for callFactExample"
