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
import Debug.Trace (trace)

parseJSONDocument :: Parser Document
parseJSONDocument = do
    skipWhitespace
    doc <- documentWithBraces
    skipWhitespace
    return doc

documentWithBraces :: Parser Document
documentWithBraces = do
    _ <- parseChar '{'
    skipWhitespace
    doc <- documentContent
    skipWhitespace
    _ <- parseChar '}'
    return doc

documentContent :: Parser Document
documentContent = do
    h <- parseJSONDocumentHeader
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    b <- parseJSONDocumentBody
    return $ Document h b

parseJSONDocumentHeader :: Parser Header
parseJSONDocumentHeader = do
    _ <- parseString "\"header\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    h <- parseJSONHeader
    return h

parseJSONDocumentBody :: Parser [Content]
parseJSONDocumentBody = do
    _ <- parseString "\"body\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseJSONBody

parseJSONHeader :: Parser Header
parseJSONHeader = do
    _ <- parseChar '{'
    skipWhitespace
    fields <- parseHeaderFields
    skipWhitespace
    _ <- parseChar '}'
    return $ createHeaderFromFields fields

parseHeaderFields :: Parser [(String, String)]
parseHeaderFields = do
    first <- parseHeaderField
    rest <- parseMany parseAdditionalHeaderField
    return (first : rest)

parseHeaderField :: Parser (String, String)
parseHeaderField = do
    _ <- parseChar '"'
    name <- parseUntil (== '"')
    _ <- parseChar '"'
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    value <- parseStringLiteral
    return (name, value)

parseAdditionalHeaderField :: Parser (String, String)
parseAdditionalHeaderField = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseHeaderField

createHeaderFromFields :: [(String, String)] -> Header
createHeaderFromFields fields =
    Header 
        (findField "title" fields)
        (lookupField "author" fields)
        (lookupField "date" fields)
  where
    findField name fs = maybe "" id (lookupField name fs)
    lookupField name fs = lookup name fs

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
    first <- parseJSONContentItem
    rest <- parseMany parseMoreContents
    return (first : rest)

parseMoreContents :: Parser Content
parseMoreContents = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseJSONContentItem

parseJSONContentItem :: Parser Content
parseJSONContentItem = parseJSONObjectContent <|> parseJSONArrayContent

parseJSONObjectContent :: Parser Content
parseJSONObjectContent = do
    _ <- parseChar '{'
    skipWhitespace
    content <- parseObjectContent
    skipWhitespace
    _ <- parseChar '}'
    return content

parseObjectContent :: Parser Content
parseObjectContent = 
    parseSectionObject <|>
    parseCodeBlockObject <|>
    parseListObject

parseSectionObject :: Parser Content
parseSectionObject = do
    _ <- parseString "\"section\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseSection

parseSection :: Parser Content
parseSection = parseJSONObject sectionParser
  where
    sectionParser = do
      title <- parseNamedField "title" parseStringLiteral
      _ <- parseChar ','
      skipWhitespace
      content <- parseNamedField "content" parseJSONBody
      return $ Section (Just title) content

parseJSONObject :: Parser a -> Parser a
parseJSONObject contentParser = do
    _ <- parseChar '{'
    skipWhitespace
    result <- contentParser
    skipWhitespace
    _ <- parseChar '}'
    return result

parseNamedField :: String -> Parser a -> Parser a
parseNamedField fieldName valueParser = do
    _ <- parseString ("\"" ++ fieldName ++ "\"")
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    valueParser

parseSectionContent :: Parser [Content]
parseSectionContent = do
    _ <- parseString "\"content\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseJSONBody

parseSectionTitle :: Parser String
parseSectionTitle = do
    _ <- parseString "\"title\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseStringLiteral

parseCodeBlockObject :: Parser Content
parseCodeBlockObject = do
    _ <- parseString "\"codeblock\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseCodeBlockArray

parseCodeBlockArray :: Parser Content
parseCodeBlockArray = do
    _ <- parseChar '['
    skipWhitespace
    lines <- parseCodeBlockLines
    skipWhitespace
    _ <- parseChar ']'
    return $ CodeBlock (unlines lines)

parseCodeBlockLines :: Parser [String]
parseCodeBlockLines = parseListOfCodeLines <|> return []

parseListOfCodeLines :: Parser [String]
parseListOfCodeLines = do
    first <- parseStringLiteral
    rest <- parseMany parseMoreCodeLines
    return (first : rest)

parseMoreCodeLines :: Parser String
parseMoreCodeLines = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseStringLiteral

parseListObject :: Parser Content
parseListObject = do
    _ <- parseString "\"list\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseListArray

parseListArray :: Parser Content
parseListArray = do
    _ <- parseChar '['
    skipWhitespace
    items <- parseListItems
    skipWhitespace
    _ <- parseChar ']'
    return $ List items

parseListItems :: Parser [Item]
parseListItems = parseListOfItems <|> return []

parseListOfItems :: Parser [Item]
parseListOfItems = do
    first <- parseListItem
    rest <- parseMany parseMoreListItems
    return (first : rest)

parseMoreListItems :: Parser Item
parseMoreListItems = do
    skipWhitespace
    _ <- parseChar ','
    skipWhitespace
    parseListItem

parseListItem :: Parser Item
parseListItem = do
    _ <- parseChar '['
    skipWhitespace
    elements <- parseJSONElements
    skipWhitespace
    _ <- parseChar ']'
    return $ Item elements

parseJSONArrayContent :: Parser Content
parseJSONArrayContent = do
    _ <- parseChar '['
    skipWhitespace
    elements <- parseJSONElements
    skipWhitespace
    _ <- parseChar ']'
    return $ Paragraph elements

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
parseJSONElement = parseJSONString <|> parseJSONObjectElement

parseJSONString :: Parser Element
parseJSONString = Text <$> parseStringLiteral

parseJSONObjectElement :: Parser Element
parseJSONObjectElement = do
    _ <- parseChar '{'
    skipWhitespace
    element <- parseJSONObjectElementContent
    skipWhitespace
    _ <- parseChar '}'
    return element

parseJSONObjectElementContent :: Parser Element
parseJSONObjectElementContent = 
    parseBoldObject <|>
    parseItalicObject <|>
    parseCodeObject <|>
    parseLinkObject <|>
    parseImageObject

parseBoldObject :: Parser Element
parseBoldObject = do
    _ <- parseString "\"bold\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    text <- parseStringLiteral
    return $ Bold [Text text]

parseItalicObject :: Parser Element
parseItalicObject = do
    _ <- parseString "\"italic\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    text <- parseStringLiteral
    return $ Italic [Text text]

parseCodeObject :: Parser Element
parseCodeObject = do
    _ <- parseString "\"code\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    code <- parseStringLiteral
    return $ Code code

parseLinkObject :: Parser Element
parseLinkObject = do
    _ <- parseString "\"link\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseLinkObjectContent

parseLinkObjectContent :: Parser Element
parseLinkObjectContent = parseJSONObject $ do
    url <- parseNamedField "url" parseStringLiteral
    _ <- parseChar ','
    skipWhitespace
    text <- parseNamedField "content" parseBracketedString
    return $ Link text url

parseBracketedString :: Parser String
parseBracketedString = do
    _ <- parseChar '['
    skipWhitespace
    text <- parseStringLiteral
    skipWhitespace
    _ <- parseChar ']'
    return text

parseLinkUrl :: Parser String
parseLinkUrl = do
    _ <- parseString "\"url\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseStringLiteral

parseLinkContent :: Parser String
parseLinkContent = parseNamedField "content" parseBracketedString

parseImageObject :: Parser Element
parseImageObject = do
    _ <- parseString "\"image\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseImageObjectContent

parseImageObjectContent :: Parser Element
parseImageObjectContent = parseJSONObject $ do
    url <- parseNamedField "url" parseStringLiteral
    _ <- parseChar ','
    skipWhitespace
    alt <- parseNamedField "alt" parseBracketedString
    return $ Image alt url

parseImageUrl :: Parser String
parseImageUrl = do
    _ <- parseString "\"url\""
    skipWhitespace
    _ <- parseChar ':'
    skipWhitespace
    parseStringLiteral

parseImageAlt :: Parser String
parseImageAlt = parseNamedField "alt" parseBracketedString