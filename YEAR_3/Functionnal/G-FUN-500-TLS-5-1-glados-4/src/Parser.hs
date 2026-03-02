{-
-- EPITECH PROJECT, 2025
-- G-FUN-500-TLS-5-1-glados-4 [WSL: Debian]
-- File description:
-- Parser
-}

module Parser
    ( parseProgram
    , sexpr
    , atom
    , parseInt
    , parseBool
    , parseSymbol
    , list
    , parseAndConvert
    ) where

import AST
import Text.Parsec
import Text.Parsec.String (Parser)

parseProgram :: String -> Either ParseError [SExpr]
parseProgram = parse (spaces *> many (sexpr <* spaces) <* eof) "input"

-- Parse S-expression
sexpr :: Parser SExpr
sexpr = spaces *> (atom <|> list) <* spaces

-- Parse atom
atom :: Parser SExpr
atom = try parseInt <|> try parseBool <|> parseSymbol

-- Parse int
parseInt :: Parser SExpr
parseInt = do
    sign <- optionMaybe (char '-')
    digits <- many1 digit
    let n = read digits :: Integer
    return $ SInt $ case sign of
        Just '-' -> -n
        _ -> n

-- Parse boolean
parseBool :: Parser SExpr
parseBool = do
    _ <- char '#'
    c <- char 't' <|> char 'f'
    return $ SBool (c == 't')

-- Parse symbol
parseSymbol :: Parser SExpr
parseSymbol = do
    first <- letter <|> symbolChar
    rest <- many (alphaNum <|> symbolChar)
    return $ SSymbol (first : rest)
  where
    symbolChar = oneOf "+-*/<>=?!_"

-- Parse list
list :: Parser SExpr
list = do
    _ <- char '('
    spaces
    elements <- many (sexpr <* spaces)
    _ <- char ')'
    return $ SList elements

-- Parse and convert to AST
parseAndConvert :: String -> Either String [AST]
parseAndConvert input = case parseProgram input of
    Left err -> Left $ show err
    Right sexprs -> mapM sexprToAST sexprs