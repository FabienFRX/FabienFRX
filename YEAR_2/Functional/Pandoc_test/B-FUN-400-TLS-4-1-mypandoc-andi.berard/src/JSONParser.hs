{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- JSONParser
-}

module JSONParser (parseJSONDocument) where

import Parser
import Document
import Control.Applicative ((<|>))

parseJSONDocument :: Parser Document
parseJSONDocument = do
    _ <- parseChar '{'
    skipWhitespace
    _ <- parseJsonField "header"
    h <- parseJSONHeader
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    _ <- parseJsonField "body"
    b <- parseJSONBody
    skipWhitespace
    _ <- parseChar '}'
    return $ Document h b

parseJsonField :: String -> Parser ()
parseJsonField name = do
    _ <- parseString ("\"" ++ name ++ "\"")
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace

parseJSONHeader :: Parser Header
parseJSONHeader = do
    _ <- parseChar '{'
    skipWhitespace
    _ <- parseJsonField "title"
    t <- parseStringLiteral
    a <- parseOptionalJSONField "author"
    d <- parseOptionalJSONField "date"
    skipWhitespace
    _ <- parseChar '}'
    return $ Header t a d

parseOptionalJSONField :: String -> Parser (Maybe String)
parseOptionalJSONField name = parseField <|> return Nothing
  where
    parseField = do
        skipWhitespace
        _ <- parseChar ','
        skipWhitespace
        _ <- parseJsonField name
        Just <$> parseStringLiteral

parseJSONBody :: Parser [Content]
parseJSONBody = do
    _ <- parseChar '['
    skipWhitespace
    contents <- parseJSONContents
    skipWhitespace
    _ <- parseChar ']'
    return contents

parseJSONContents :: Parser [Content]
parseJSONContents = parseListOfContents <|> return []

parseListOfContents :: Parser [Content]
parseListOfContents = do
    first <- parseJSONContent
    rest <- parseMany parseMoreContents
    return (first : rest)

parseMoreContents :: Parser Content
parseMoreContents = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseJSONContent

parseJSONContent :: Parser Content
parseJSONContent = do
    _ <- parseChar '{'
    skipWhitespace
    _ <- parseJsonField "type"
    contentType <- parseStringLiteral
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    content <- parseContentByType contentType
    skipWhitespace
    _ <- parseChar '}'
    return content

parseContentByType :: String -> Parser Content
parseContentByType "paragraph" = parseParagraphContent
parseContentByType "section" = parseSectionContent
parseContentByType "codeblock" = parseCodeBlockContent
parseContentByType "list" = parseListContent
parseContentByType _ = Parser $ \_ -> Nothing

parseParagraphContent :: Parser Content
parseParagraphContent = do
    _ <- parseJsonField "content"
    elements <- parseJSONElementArray
    return $ Paragraph elements

parseSectionContent :: Parser Content
parseSectionContent = do
    t <- parseOptionalSectionTitle
    skipWhitespace
    _ <- parseJsonField "content"
    _ <- parseChar '['
    skipWhitespace
    contents <- parseJSONContents
    skipWhitespace
    _ <- parseChar ']'
    return $ Section t contents

parseOptionalSectionTitle :: Parser (Maybe String)
parseOptionalSectionTitle = parseTitle <|> return Nothing
  where
    parseTitle = do
        _ <- parseJsonField "title"
        t <- parseStringLiteral
        skipWhitespace
        _ <- parseChar ','
        skipWhitespace
        return $ Just t

parseCodeBlockContent :: Parser Content
parseCodeBlockContent = do
    _ <- parseJsonField "content"
    CodeBlock <$> parseStringLiteral

parseListContent :: Parser Content
parseListContent = do
    _ <- parseJsonField "items"
    _ <- parseChar '['
    skipWhitespace
    items <- parseJSONItems
    skipWhitespace
    _ <- parseChar ']'
    return $ List items

parseJSONItems :: Parser [Item]
parseJSONItems = parseListOfItems <|> return []

parseListOfItems :: Parser [Item]
parseListOfItems = do
    first <- parseJSONItem
    rest <- parseMany parseMoreItems
    return (first : rest)

parseMoreItems :: Parser Item
parseMoreItems = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseJSONItem

parseJSONItem :: Parser Item
parseJSONItem = do
    _ <- parseChar '{'
    skipWhitespace
    _ <- parseJsonField "content"
    _ <- parseChar '['
    skipWhitespace
    elements <- parseJSONElements
    skipWhitespace
    _ <- parseChar ']'
    skipWhitespace
    _ <- parseChar '}'
    return $ Item elements

parseJSONElementArray :: Parser [Element]
parseJSONElementArray = do
    _ <- parseChar '['
    skipWhitespace
    elements <- parseJSONElements
    skipWhitespace
    _ <- parseChar ']'
    return elements

parseJSONElements :: Parser [Element]
parseJSONElements = parseListOfElements <|> return []

parseListOfElements :: Parser [Element]
parseListOfElements = do
    first <- parseJSONElement
    rest <- parseMany parseMoreElements
    return (first : rest)

parseMoreElements :: Parser Element
parseMoreElements = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseJSONElement

parseJSONElement :: Parser Element
parseJSONElement = do
    _ <- parseChar '{'
    skipWhitespace
    _ <- parseJsonField "type"
    elementType <- parseStringLiteral
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    element <- parseElementByType elementType
    skipWhitespace
    _ <- parseChar '}'
    return element

parseElementByType :: String -> Parser Element
parseElementByType "text" = parseTextElement
parseElementByType "italic" = parseItalicElement
parseElementByType "bold" = parseBoldElement
parseElementByType "code" = parseCodeElement
parseElementByType "link" = parseLinkElement
parseElementByType "image" = parseImageElement
parseElementByType _ = Parser $ \_ -> Nothing

parseTextElement :: Parser Element
parseTextElement = do
    _ <- parseJsonField "content"
    Text <$> parseStringLiteral

parseItalicElement :: Parser Element
parseItalicElement = do
    _ <- parseJsonField "content"
    elements <- parseJSONElementArray
    return $ Italic elements

parseBoldElement :: Parser Element
parseBoldElement = do
    _ <- parseJsonField "content"
    elements <- parseJSONElementArray
    return $ Bold elements

parseCodeElement :: Parser Element
parseCodeElement = do
    _ <- parseJsonField "content"
    Code <$> parseStringLiteral

parseLinkElement :: Parser Element
parseLinkElement = parseComplexElement Link "text" "url"

parseImageElement :: Parser Element
parseImageElement = parseComplexElement Image "alt" "url"

parseComplexElement :: (String -> String -> Element) -> String -> String -> Parser Element
parseComplexElement constructor field1 field2 = do
    _ <- parseJsonField field1
    value1 <- parseStringLiteral
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    _ <- parseJsonField field2
    value2 <- parseStringLiteral
    return $ constructor value1 value2