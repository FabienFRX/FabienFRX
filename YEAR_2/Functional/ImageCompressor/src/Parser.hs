{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- Parser
-}

module Parser (parsePixels, ParseError(..)) where

import Types

data ParseError = InvalidRGBRange | InvalidFormat
  deriving (Show, Eq)

validateRGB :: Color -> Bool
validateRGB (r, g, b) = 
  r >= 0 && r <= 255 && 
  g >= 0 && g <= 255 && 
  b >= 0 && b <= 255

parseComponentPair :: (String -> Maybe a) -> String -> Either ParseError a
parseComponentPair parser str = case parser str of
  Just value -> Right value
  Nothing    -> Left InvalidFormat

parseLine :: String -> Either ParseError Pixel
parseLine line = case words line of
  [pointStr, colorStr] -> do
    point <- parseComponentPair parsePoint pointStr
    color <- parseComponentPair parseColor colorStr
    return (point, color)
  _ -> Left InvalidFormat

parsePoint :: String -> Maybe Point
parsePoint str = case reads str of
  [(point, "")] -> Just point
  _             -> Nothing

parseColor :: String -> Maybe Color
parseColor str = case reads str of
  [(color, "")] -> if validateRGB color 
                   then Just color 
                   else Nothing
  _             -> Nothing

parsePixels :: String -> Either ParseError [Pixel]
parsePixels content = 
  let results = map parseLine (lines content)
      errors = [e | Left e <- results]
      parsedPixels = [p | Right p <- results]
  in case errors of
       (err:_) -> Left err
       [] -> Right parsedPixels