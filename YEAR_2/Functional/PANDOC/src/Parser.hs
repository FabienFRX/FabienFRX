{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- Parser
-}

module Parser
    ( Parser(..)
    , parseChar
    , parseAnyChar
    , parseOr
    , parseAnd
    , parseAndWith
    , parseMany
    , parseSome
    , parseUInt
    , parseInt
    , parseTuple
    , parseString
    , skipWhitespace
    , parseStringLiteral
    , parseUntil
    , parseWhile
    ) where

import Control.Applicative (Alternative(..))

data Parser a = Parser { runParser :: String -> Maybe (a, String) }

instance Functor Parser where
    fmap f p = Parser $ \input -> case runParser p input of
        Nothing -> Nothing
        Just (x, rest) -> Just (f x, rest)

instance Applicative Parser where
    pure x = Parser $ \input -> Just (x, input)
    pf <*> px = Parser $ \input -> case runParser pf input of
        Nothing -> Nothing
        Just (f, rest) -> runParser (fmap f px) rest

instance Alternative Parser where
    empty = Parser $ const Nothing
    p1 <|> p2 = Parser $ \input -> case runParser p1 input of
        Nothing -> runParser p2 input
        result -> result

instance Monad Parser where
    return = pure
    p >>= f = Parser $ \input -> case runParser p input of
        Nothing -> Nothing
        Just (x, rest) -> runParser (f x) rest

parseChar :: Char -> Parser Char
parseChar c = Parser parse
  where
    parse (x:xs)
      | x == c    = Just (c, xs)
      | otherwise = Nothing
    parse _ = Nothing

parseAnyChar :: String -> Parser Char
parseAnyChar cs = Parser parse
  where
    parse (x:xs)
      | x `elem` cs = Just (x, xs)
      | otherwise   = Nothing
    parse _ = Nothing

parseOr :: Parser a -> Parser a -> Parser a
parseOr = (<|>)

parseAnd :: Parser a -> Parser b -> Parser (a, b)
parseAnd pa pb = (,) <$> pa <*> pb

parseAndWith :: (a -> b -> c) -> Parser a -> Parser b -> Parser c
parseAndWith f pa pb = f <$> pa <*> pb

parseMany :: Parser a -> Parser [a]
parseMany p = parseSome p <|> pure []

parseSome :: Parser a -> Parser [a]
parseSome p = (:) <$> p <*> parseMany p

parseUInt :: Parser Int
parseUInt = read <$> parseSome (parseAnyChar ['0'..'9'])

parseInt :: Parser Int
parseInt = do
    sign <- parseChar '-' <|> return '+'
    n <- parseUInt
    return $ if sign == '-' then -n else n

parseTuple :: Parser a -> Parser (a, a)
parseTuple p = do
    _ <- parseChar '('
    a <- p
    _ <- parseChar ','
    b <- p
    _ <- parseChar ')'
    return (a, b)

parseString :: String -> Parser String
parseString str = Parser parse
  where
    parse input
      | take len input == str = Just (str, drop len input)
      | otherwise = Nothing
    len = length str

skipWhitespace :: Parser ()
skipWhitespace = parseMany (parseAnyChar " \t\n\r") >> return ()

parseStringLiteral :: Parser String
parseStringLiteral = do
    _ <- parseChar '"'
    content <- parseMany parseStringChar
    _ <- parseChar '"'
    return content

parseStringChar :: Parser Char
parseStringChar = parseEscapedChar <|> parseNormalChar

parseEscapedChar :: Parser Char
parseEscapedChar = do
    _ <- parseChar '\\'
    c <- parseAnyChar "\"\\/bfnrt"
    return $ case c of
        '"' -> '"'
        '\\' -> '\\'
        '/' -> '/'
        'b' -> '\b'
        'f' -> '\f'
        'n' -> '\n'
        'r' -> '\r'
        't' -> '\t'
        _ -> c

parseNormalChar :: Parser Char
parseNormalChar = parseAnyChar validChars
  where
    validChars = filter (\c -> c /= '"' && c /= '\\') [' '..'\127']

parseUntil :: (Char -> Bool) -> Parser String
parseUntil pred = Parser parse
  where
    parse input = 
        let (taken, rest) = span (not . pred) input
        in Just (taken, rest)

parseWhile :: (Char -> Bool) -> Parser String
parseWhile pred = Parser parse
  where
    parse input = 
        let (taken, rest) = span pred input
        in Just (taken, rest)