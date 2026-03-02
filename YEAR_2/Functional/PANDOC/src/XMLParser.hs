{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard
-- File description:
-- XMLParser
-}

module XMLParser (parseXMLDocument) where

import Parser
import Document
import Control.Applicative ((<|>))

parseXMLDocument :: Parser Document
parseXMLDocument = 
    skipWhitespace >>
    documentWithTags "<document>" "</document>" parseXMLDocumentContent

parseXMLDocumentContent :: Parser Document
parseXMLDocumentContent = do
    header <- parseXMLHeader
    skipWhitespace
    body <- parseXMLBody
    return $ Document header body

documentWithTags :: String -> String -> Parser a -> Parser a
documentWithTags openTag closeTag parser = do
    _ <- parseString openTag
    skipWhitespace
    result <- parser
    skipWhitespace
    _ <- parseString closeTag
    skipWhitespace
    return result

parseXMLHeader :: Parser Header
parseXMLHeader = do
    _ <- parseString "<header"
    attrs <- parseAttributes
    inner <- parseHeaderInner
    return $ createHeader attrs inner

parseAttributes :: Parser [(String, String)]
parseAttributes = do
    skipWhitespace
    attrs <- parseAttributeList
    _ <- parseChar '>'
    return attrs

parseAttributeList :: Parser [(String, String)]
parseAttributeList = parseMany parseAttribute

parseAttribute :: Parser (String, String)
parseAttribute = do
    skipWhitespace
    name <- parseSome (parseAnyChar validNameChars)
    skipWhitespace
    _ <- parseChar '='
    skipWhitespace
    value <- parseQuotedString
    return (name, value)
  where
    validNameChars = ['a'..'z'] ++ ['A'..'Z'] ++ ['0'..'9'] ++ "_-"

parseQuotedString :: Parser String
parseQuotedString = do
    _ <- parseChar '"'
    content <- parseMany (parseAnyChar (filter (/= '"') [' '..'\127']))
    _ <- parseChar '"'
    return content

parseHeaderInner :: Parser [(String, String)]
parseHeaderInner = do
    skipWhitespace
    elements <- parseMany parseHeaderElement
    skipWhitespace
    _ <- parseString "</header>"
    return elements

parseHeaderElement :: Parser (String, String)
parseHeaderElement = do
    skipWhitespace
    _ <- parseChar '<'
    tagName <- parseSome (parseAnyChar ['a'..'z'])
    _ <- parseChar '>'
    content <- parseMany (parseAnyChar (filter (/= '<') [' '..'\127']))
    _ <- parseString ("</" ++ tagName ++ ">")
    skipWhitespace
    return (tagName, content)

createHeader :: [(String, String)] -> [(String, String)] -> Header
createHeader attrs elements =
    Header
        (lookup "title" attrs `orElse` "")
        (lookup "author" elements <|> lookup "author" attrs)
        (lookup "date" elements <|> lookup "date" attrs)
  where
    Nothing `orElse` def = def
    Just x `orElse` _ = x

parseXMLBody :: Parser [Content]
parseXMLBody = do
    _ <- parseString "<body>"
    skipWhitespace
    contents <- parseMany parseBodyContent
    skipWhitespace
    _ <- parseString "</body>"
    return contents

parseBodyContent :: Parser Content
parseBodyContent = 
    parseParagraph <|> 
    parseSection <|> 
    parseCodeBlock <|> 
    parseList

parseParagraph :: Parser Content
parseParagraph = do
    skipWhitespace
    _ <- parseString "<paragraph>"
    elements <- parseMany parseElement
    _ <- parseString "</paragraph>"
    skipWhitespace
    return $ Paragraph elements

parseSection :: Parser Content
parseSection = do
    skipWhitespace
    _ <- parseString "<section"
    attrs <- parseAttributes
    skipWhitespace
    contents <- parseMany parseSectionContent
    skipWhitespace
    _ <- parseString "</section>"
    skipWhitespace
    return $ Section (lookup "title" attrs) contents

parseSectionContent :: Parser Content
parseSectionContent = 
    parseParagraph <|> 
    parseSection <|> 
    parseCodeBlock <|> 
    parseList

parseCodeBlock :: Parser Content
parseCodeBlock = do
    skipWhitespace
    _ <- parseString "<codeblock>"
    skipWhitespace
    content <- parseCodeBlockContent
    skipWhitespace
    _ <- parseString "</codeblock>"
    skipWhitespace
    return $ CodeBlock content

parseCodeBlockContent :: Parser String
parseCodeBlockContent = parseParagraphContent <|> parseRawContent

parseParagraphContent :: Parser String
parseParagraphContent = do
    _ <- parseString "<paragraph>"
    text <- parseMany (parseAnyChar (filter (/= '<') [' '..'\127']))
    _ <- parseString "</paragraph>"
    return text

parseRawContent :: Parser String
parseRawContent = parseMany (parseAnyChar (filter (/= '<') [' '..'\127']))

parseList :: Parser Content
parseList = do
    skipWhitespace
    _ <- parseString "<list>"
    skipWhitespace
    items <- parseMany parseListItem
    skipWhitespace
    _ <- parseString "</list>"
    skipWhitespace
    return $ List items

parseListItem :: Parser Item
parseListItem = do
    skipWhitespace
    _ <- parseString "<paragraph>"
    elements <- parseMany parseElement
    _ <- parseString "</paragraph>"
    skipWhitespace
    return $ Item elements

parseElement :: Parser Element
parseElement = parseTextElement <|> parseFormattedElement

parseTextElement :: Parser Element
parseTextElement = do
    text <- parseSome (parseAnyChar (filter (`notElem` "<>") [' '..'\127']))
    return $ Text text

parseFormattedElement :: Parser Element
parseFormattedElement = 
    parseBold <|> parseItalic <|> parseCode <|> parseLink <|> parseImage

parseBold :: Parser Element
parseBold = do
    _ <- parseString "<bold>"
    elements <- parseMany parseElement
    _ <- parseString "</bold>"
    return $ Bold elements

parseItalic :: Parser Element
parseItalic = do
    _ <- parseString "<italic>"
    elements <- parseMany parseElement
    _ <- parseString "</italic>"
    return $ Italic elements

parseCode :: Parser Element
parseCode = do
    _ <- parseString "<code>"
    code <- parseMany (parseAnyChar (filter (/= '<') [' '..'\127']))
    _ <- parseString "</code>"
    return $ Code code

parseLink :: Parser Element
parseLink = do
    _ <- parseString "<link"
    attrs <- parseAttributes
    text <- parseMany (parseAnyChar (filter (/= '<') [' '..'\127']))
    _ <- parseString "</link>"
    return $ Link text (lookup "url" attrs `orElse` "")
  where
    Nothing `orElse` def = def
    Just x `orElse` _ = x

parseImage :: Parser Element
parseImage = do
    _ <- parseString "<image"
    attrs <- parseAttributes
    alt <- parseMany (parseAnyChar (filter (/= '<') [' '..'\127']))
    _ <- parseString "</image>"
    return $ Image alt (lookup "url" attrs `orElse` "")
  where
    Nothing `orElse` def = def
    Just x `orElse` _ = x