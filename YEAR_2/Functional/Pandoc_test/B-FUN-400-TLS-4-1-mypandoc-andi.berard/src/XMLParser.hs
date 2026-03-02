{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- XMLParser
-}

module XMLParser (parseXMLDocument) where

import Parser
import Document
import Control.Applicative ((<|>))

parseXMLDocument :: Parser Document
parseXMLDocument = do
    _ <- parseString "<document>"
    skipWhitespace
    h <- parseXMLHeader
    skipWhitespace
    b <- parseXMLBody
    skipWhitespace
    _ <- parseString "</document>"
    return (Document h b)

parseXMLHeader :: Parser Header
parseXMLHeader = parseHeaderOpen >> parseHeaderContent

parseHeaderContent :: Parser Header
parseHeaderContent = do
    t <- parseAttribute "title"
    a <- parseOptionalAttribute "author"
    d <- parseOptionalAttribute "date"
    parseHeaderClose
    return (Header t a d)

parseHeaderOpen :: Parser ()
parseHeaderOpen = parseString "<header" >> skipWhitespace

parseHeaderClose :: Parser ()
parseHeaderClose = do
    skipWhitespace 
    _ <- parseString ">" 
    skipWhitespace 
    _ <- parseString "</header>"
    return ()

parseXMLBody :: Parser [Content]
parseXMLBody = do
    _ <- parseString "<body>"
    skipWhitespace
    contents <- parseMany parseXMLContent
    skipWhitespace
    _ <- parseString "</body>"
    return contents

parseXMLContent :: Parser Content
parseXMLContent = do
    skipWhitespace
    content <- parseParagraph <|> parseSection <|> parseCodeBlock <|> parseList
    skipWhitespace
    return content

parseAttribute :: String -> Parser String
parseAttribute name = do
    _ <- parseString name
    _ <- parseChar '='
    _ <- parseChar '"'
    value <- parseMany (parseAnyChar validChars)
    _ <- parseChar '"'
    return value
  where
    validChars = ['a'..'z'] ++ ['A'..'Z'] ++ ['0'..'9'] ++ " _-.,;:!?"

parseOptionalAttribute :: String -> Parser (Maybe String)
parseOptionalAttribute name = 
    (parseSpace >> Just <$> parseAttribute name) <|> return Nothing
  where
    parseSpace = parseChar ' ' >> return ()

parseParagraph :: Parser Content
parseParagraph = do
    _ <- parseString "<paragraph>"
    elements <- parseMany parseXMLElement
    _ <- parseString "</paragraph>"
    return (Paragraph elements)

parseSection :: Parser Content
parseSection = do
    _ <- parseString "<section"
    title <- parseSectionTitle
    _ <- parseString ">"
    contents <- parseSectionContents
    _ <- parseString "</section>"
    return (Section title contents)

parseSectionTitle :: Parser (Maybe String)
parseSectionTitle = parseOptionalAttribute "title"

parseSectionContents :: Parser [Content]
parseSectionContents = do
    skipWhitespace
    contents <- parseMany parseXMLContent
    skipWhitespace
    return contents

parseCodeBlock :: Parser Content
parseCodeBlock = do
    _ <- parseString "<codeblock>"
    code <- parseMany (parseAnyChar validChars)
    _ <- parseString "</codeblock>"
    return (CodeBlock code)
  where
    validChars = ['\t'..'\127']

parseList :: Parser Content
parseList = do
    _ <- parseString "<list>"
    skipWhitespace
    items <- parseMany parseItem
    skipWhitespace
    _ <- parseString "</list>"
    return (List items)

parseItem :: Parser Item
parseItem = do
    skipWhitespace
    _ <- parseString "<item>"
    elements <- parseMany parseXMLElement
    _ <- parseString "</item>"
    skipWhitespace
    return (Item elements)

parseXMLElement :: Parser Element
parseXMLElement = 
    parseText <|> 
    parseItalic <|> 
    parseBold <|> 
    parseCode <|> 
    parseLink <|> 
    parseImage

parseText :: Parser Element
parseText = Text <$> parseSome (parseAnyChar validChars)
  where
    validChars = filter (`notElem` "<>") [' '..'\127']

parseItalic :: Parser Element
parseItalic = do
    _ <- parseString "<italic>"
    elements <- parseMany parseXMLElement
    _ <- parseString "</italic>"
    return (Italic elements)

parseBold :: Parser Element
parseBold = do
    _ <- parseString "<bold>"
    elements <- parseMany parseXMLElement
    _ <- parseString "</bold>"
    return (Bold elements)

parseCode :: Parser Element
parseCode = do
    _ <- parseString "<code>"
    code <- parseMany (parseAnyChar validChars)
    _ <- parseString "</code>"
    return (Code code)
  where
    validChars = filter (`notElem` "<>") [' '..'\127']

parseLink :: Parser Element
parseLink = do
    _ <- parseString "<link url=\""
    url <- parseMany (parseAnyChar validChars)
    _ <- parseString "\">"
    text <- parseMany (parseAnyChar validChars)
    _ <- parseString "</link>"
    return (Link text url)
  where
    validChars = filter (`notElem` "<>\"") [' '..'\127']

parseImage :: Parser Element
parseImage = do
    _ <- parseString "<image url=\""
    url <- parseMany (parseAnyChar validChars)
    _ <- parseString "\" alt=\""
    alt <- parseMany (parseAnyChar validChars)
    _ <- parseString "\"/>"
    return (Image alt url)
  where
    validChars = filter (`notElem` "<>\"") [' '..'\127']