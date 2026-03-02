{-
-- EPITECH PROJECT, 2025
-- MyPandoc
-- File description:
-- Main
-}

data Parser a = Parser {
    runParser :: String -> Maybe (a, String)
}

parseChar :: Parser Char -> Parser Char
parseChar _ [] = Nothing
parseChar c (x:xs)
    | c == x = Just (c, xs)
    | otherwise = Nothing

parseAnyChar :: String -> Parser Char
parseAnyChar [] _ = Nothing
parseAnyChar _ [] = Nothing
parseAnyChar (x:xs) (y:ys)
    | x == y = Just (x, ys)
    | otherwise = parseAnyChar xs (y:ys)

parseOr :: Parser a -> Parser a -> Parser a
parseOr p1 p2 input = case p1 input of
    Just result -> Just result
    Nothing     -> p2 input

parseAnyChar' :: String -> Parser Char
parseAnyChar' [] _ = Nothing
parseAnyChar' (c:cs) input = parseOr (parseChar c) (parseAnyChar' cs) input

parseAnd :: Parser a -> Parser b -> Parser (a, b)
parseAnd p1 p2 input = case p1 input of
    Just (result1, rest1) -> case p2 rest1 of
        Just (result2, rest2) -> Just ((result1, result2), rest2)
        Nothing -> Nothing
    Nothing -> Nothing

parseAndWith :: (a -> b -> c) -> Parser a -> Parser b -> Parser c
parseAndWith f p1 p2 input = case p1 input of
    Just (result1, rest1) -> case p2 rest1 of
        Just (result2, rest2) -> Just (f result1 result2, rest2)
        Nothing -> Nothing
    Nothing -> Nothing

parseMany :: Parser a -> Parser [a]
parseMany p input = case p input of
    Just (result, rest) -> case parseMany p rest of
        Just (results, finalRest) -> Just (result:results, finalRest)
        Nothing -> Nothing
    Nothing -> Just ([], input)

parseSome :: Parser a -> Parser [a]
parseSome p = parseAndWith (:) p (parseMany p)

parseUInt :: Parser Int
parseUInt input = case parseSome (parseAnyChar ['0'..'9']) input of
    Just (digits, rest) -> Just (read digits, rest)
    Nothing -> Nothing

parseInt :: Parser Int
parseInt input = case parseChar '-' input of
    Just (_, rest) -> case parseUInt rest of
        Just (num, finalRest) -> Just (-num, finalRest)
        Nothing -> Nothing
    Nothing -> parseUInt input

parseTuple :: Parser a -> Parser (a, a)
parseTuple p input = case parseChar '(' input of
    Just (_, rest1) -> case p rest1 of
        Just (val1, rest2) -> case parseChar ',' rest2 of
            Just (_, rest3) -> case p rest3 of
                Just (val2, rest4) -> case parseChar ')' rest4 of
                    Just (_, finalRest) -> Just ((val1, val2), finalRest)
                    Nothing -> Nothing
                Nothing -> Nothing
            Nothing -> Nothing
        Nothing -> Nothing
    Nothing -> Nothing