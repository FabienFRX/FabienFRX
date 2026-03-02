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
    ) where

import Control.Applicative (Alternative(..))
-- No external imports needed

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
parseChar c = Parser $ \input -> case input of
    (x:xs) | x == c -> Just (c, xs)
    _ -> Nothing

parseAnyChar :: String -> Parser Char
parseAnyChar cs = Parser $ \input -> case input of
    (x:xs) | x `elem` cs -> Just (x, xs)
    _ -> Nothing

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
parseString str = Parser $ \input -> 
    let len = length str
    in if take len input == str
       then Just (str, drop len input)
       else Nothing

skipWhitespace :: Parser ()
skipWhitespace = parseMany (parseAnyChar " \t\n\r") >> return ()

parseStringLiteral :: Parser String
parseStringLiteral = do
    _ <- parseChar '"'
    contents <- parseMany (parseAnyChar validChars)
    _ <- parseChar '"'
    return contents
  where
    validChars = ['a'..'z'] ++ ['A'..'Z'] ++ ['0'..'9'] ++ " _-.,;:!?()[]{}<>@#$%^&*+=~/\\|"