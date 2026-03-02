{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- MarkdownParser
-}

module MarkdownParser (parseMarkdownDocument) where

import Parser
import Document
import Control.Applicative ((<|>))

parseMarkdownDocument :: Parser Document
parseMarkdownDocument = do
    h <- parseMarkdownHeader
    b <- parseMarkdownBody
    return $ Document h b

parseMarkdownHeader :: Parser Header
parseMarkdownHeader = do
    _ <- parseString "---"
    parseNewline
    t <- parseMarkdownField "title"
    a <- parseOptionalMarkdownField "author"
    d <- parseOptionalMarkdownField "date"
    _ <- parseString "---"
    parseNewline
    skipEmptyLines
    return $ Header t a d

parseMarkdownBody :: Parser [Content]
parseMarkdownBody = parseMany parseMarkdownContent

parseNewline :: Parser ()
parseNewline = parseChar '\n' >> return ()

skipEmptyLines :: Parser ()
skipEmptyLines = parseMany (skipWhitespace >> parseNewline) >> return ()

parseMarkdownField :: String -> Parser String
parseMarkdownField name = do
    _ <- parseString name
    _ <- parseString ": "
    value <- parseMany (parseAnyChar allowedChars)
    parseNewline
    return value
    where
        allowedChars = ['a'..'z'] ++ ['A'..'Z'] ++ ['0'..'9'] ++ " _-.,;:!?()"

parseOptionalMarkdownField :: String -> Parser (Maybe String)
parseOptionalMarkdownField name = 
    (Just <$> parseMarkdownField name) <|> return Nothing

parseMarkdownContent :: Parser Content
parseMarkdownContent = parseSection <|> parseCodeBlock <|> parseList <|> parseParagraph

parseParagraph :: Parser Content
parseParagraph = do
    linesContent <- parseSome parseParagraphLine
    skipEmptyLines
    return $ Paragraph (parseTextElements (concat linesContent))

parseParagraphLine :: Parser String
parseParagraphLine = do
    content <- parseMany (parseAnyChar (filter (/= '\n') [' '..'\127']))
    parseNewline
    return content

parseSection :: Parser Content
parseSection = do
    _ <- parseChar '#'
    _ <- parseChar ' '
    sectionTitle <- parseMany (parseAnyChar (filter (/= '\n') [' '..'\127']))
    parseNewline
    skipEmptyLines
    contents <- parseMany parseMarkdownContent
    return $ Section (Just sectionTitle) contents

parseCodeBlock :: Parser Content
parseCodeBlock = do
    _ <- parseString "```"
    parseNewline
    code <- parseMany parseCodeLine
    _ <- parseString "```"
    parseNewline
    skipEmptyLines
    return $ CodeBlock (concat code)

parseCodeLine :: Parser String
parseCodeLine = do
    line <- parseMany (parseAnyChar (filter (/= '\n') [' '..'\127']))
    parseNewline
    return (line ++ "\n")

parseList :: Parser Content
parseList = do
    items <- parseSome parseListItem
    skipEmptyLines
    return $ List items

parseListItem :: Parser Item
parseListItem = do
    _ <- parseString "- "
    content <- parseMany (parseAnyChar (filter (/= '\n') [' '..'\127']))
    parseNewline
    return $ Item (parseTextElements content)

parseTextElements :: String -> [Element]
parseTextElements [] = []
parseTextElements ('*':'*':rest) =
    let (boldText, remaining) = break (== '*') rest
    in if take 2 remaining == "**"
       then Bold [Text boldText] : parseTextElements (drop 2 remaining)
       else Text ('*':'*':boldText) : parseTextElements remaining
parseTextElements ('*':rest) =
    let (italicText, remaining) = break (== '*') rest
    in if take 1 remaining == "*"
       then Italic [Text italicText] : parseTextElements (drop 1 remaining)
       else Text ('*':italicText) : parseTextElements remaining
parseTextElements ('`':rest) =
    let (codeText, remaining) = break (== '`') rest
    in if take 1 remaining == "`"
       then Code codeText : parseTextElements (drop 1 remaining)
       else Text ('`':codeText) : parseTextElements remaining
parseTextElements ('[':rest) =
    let (linkText, remaining) = break (== ']') rest
    in if take 2 (drop 1 remaining) == "("
       then 
           let (url, urlRemaining) = break (== ')') (drop 2 remaining)
           in if take 1 urlRemaining == ")"
              then Link linkText url : parseTextElements (drop 1 urlRemaining)
              else Text ('[':linkText ++ ']' : take 1 remaining) : parseTextElements (drop 1 remaining)
       else Text ('[':linkText) : parseTextElements remaining
parseTextElements ('!':'[':rest) =
    let (altText, remaining) = break (== ']') rest
    in if take 2 (drop 1 remaining) == "("
       then 
           let (url, urlRemaining) = break (== ')') (drop 2 remaining)
           in if take 1 urlRemaining == ")"
              then Image altText url : parseTextElements (drop 1 urlRemaining)
              else Text ('!':'[':altText ++ ']' : take 1 remaining) : parseTextElements (drop 1 remaining)
       else Text ('!':'[':altText) : parseTextElements remaining
parseTextElements (c:rest) =
    let (text, remaining) = span (\x -> x /= '*' && x /= '`' && x /= '[' && x /= '!') (c:rest)
    in Text text : parseTextElements remaining