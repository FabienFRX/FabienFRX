{-
-- EPITECH PROJECT, 2025
-- GLaDOS Parser
-- File description:
-- Parser
-}

module Parser (parseProgram) where

import AST
import Text.Parsec
import Text.Parsec.String (Parser)
import Data.Char (isAlpha, isAlphaNum)

parseProgram :: String -> Either String [AST]
parseProgram input = 
    case parse (spaces *> many statement <* spaces <* eof) "" input of
        Left err -> Left $ show err
        Right ast -> Right ast

statement :: Parser AST
statement = spaces *> choice
    [ try funcDef
    , try assignment
    , try returnStmt
    , try ifStmt
    , exprStmt
    ] <* spaces

assignment :: Parser AST
assignment = do
    name <- identifier
    spaces >> char '=' >> spaces
    expr <- expression
    optional (char ';')
    return $ AAssign name expr

funcDef :: Parser AST
funcDef = do
    string "fn" >> spaces
    name <- identifier
    params <- parens (identifier `sepBy` (spaces >> char ',' >> spaces))
    body <- braces (many statement)
    return $ AFuncDef name params (ASeq body)

returnStmt :: Parser AST
returnStmt = do
    string "return" >> spaces
    expr <- expression
    optional (char ';')
    return $ AReturn expr

ifStmt :: Parser AST
ifStmt = do
    string "if" >> spaces
    cond <- parens expression
    thenBlock <- braces (many statement)
    elseBlock <- option [] $
        spaces >> string "else" >> spaces >>
        braces (many statement)
    return $ AIf cond (ASeq thenBlock) (ASeq elseBlock)

exprStmt :: Parser AST
exprStmt = do
    expr <- expression
    optional (char ';')
    return expr

expression :: Parser AST
expression = comparison

comparison :: Parser AST
comparison = do
    left <- term
    spaces
    op <- optionMaybe comparisonOp
    case op of
        Just opStr -> parseComparisonRight left opStr
        _ -> return left

comparisonOp :: Parser String
comparisonOp = try (string "<=")
           <|> try (string ">=")
           <|> try (string "==")
           <|> try (string "!=")
           <|> try (string "<")
           <|> string ">"

parseComparisonRight :: AST -> String -> Parser AST
parseComparisonRight left "==" = parseRightTerm left AEq
parseComparisonRight left "!=" = parseRightTerm left ANotEq
parseComparisonRight left "<" = parseRightTerm left ALess
parseComparisonRight left ">" = parseRightTerm left AGreater
parseComparisonRight left "<=" = parseRightTerm left ALessEq
parseComparisonRight left ">=" = parseRightTerm left AGreaterEq
parseComparisonRight left _ = return left

parseRightTerm :: AST -> (AST -> AST -> AST) -> Parser AST
parseRightTerm left constructor = do
    spaces
    right <- term
    return $ constructor left right

term :: Parser AST
term = chainl1 factor addOp
  where
    addOp = (char '+' >> spaces >> return AAdd)
        <|> (char '-' >> spaces >> return ASub)

factor :: Parser AST
factor = chainl1 primary mulOp
  where
    mulOp = (char '*' >> spaces >> return AMul)
        <|> (char '/' >> spaces >> return ADiv)

primary :: Parser AST
primary = choice
    [ try funcCall
    , try intLiteral
    , try boolLiteral
    , variable
    , parens expression
    ]

intLiteral :: Parser AST
intLiteral = do
    sign <- optionMaybe (char '-')
    n <- many1 digit
    spaces
    let value = case sign of
                  Just _  -> -(read n)
                  Nothing -> read n
    return $ AInt value

boolLiteral :: Parser AST
boolLiteral = do
    b <- string "true" <|> string "false"
    spaces
    return $ ABool (b == "true")

variable :: Parser AST
variable = AVar <$> identifier

funcCall :: Parser AST
funcCall = do
    name <- identifier
    args <- parens (expression `sepBy` (spaces >> char ',' >> spaces))
    return $ ACall name args

identifier :: Parser String
identifier = do
    first <- satisfy (\c -> isAlpha c || c == '_')
    rest <- many (satisfy (\c -> isAlphaNum c || c == '_'))
    spaces
    return (first : rest)

parens :: Parser a -> Parser a
parens p = char '(' >> spaces >> p <* spaces <* char ')' <* spaces

braces :: Parser a -> Parser a
braces p = char '{' >> spaces >> p <* spaces <* char '}' <* spaces