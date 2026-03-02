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
parseMarkdownDocument = makeDocument <$> 
    (parseMarkdownHeader <|> return emptyHeader) <*> 
    parseMarkdownBody

emptyHeader :: Header  
emptyHeader = Header "" Nothing Nothing

makeDocument :: Header -> [Content] -> Document
makeDocument = Document

parseMarkdownHeader :: Parser Header
parseMarkdownHeader = do
    _ <- parseString "---"
    _ <- parseNewline
    fields <- parseHeaderFields
    _ <- parseString "---"
    _ <- parseNewline
    skipEmptyLines
    return $ createHeader fields

createHeader :: [(String, String)] -> Header
createHeader fields = Header
    (getFieldValue "title" fields) 
    (lookup "author" fields)
    (lookup "date" fields)

getFieldValue :: String -> [(String, String)] -> String
getFieldValue key fields = maybe "" id (lookup key fields)

parseHeaderFields :: Parser [(String, String)]
parseHeaderFields = parseMany parseHeaderField

parseHeaderField :: Parser (String, String)
parseHeaderField = do
    name <- parseFieldName
    _ <- parseChar ':'
    skipHorizontalWhitespace
    value <- parseFieldValue
    _ <- parseNewline
    return (name, value)

parseFieldName :: Parser String
parseFieldName = parseSome (parseAnyChar allowedChars)
  where
    allowedChars = ['a'..'z'] ++ ['A'..'Z'] ++ ['0'..'9'] ++ "_-"

parseFieldValue :: Parser String
parseFieldValue = parseMany (parseAnyChar nonNewlineChars)
  where
    nonNewlineChars = filter (/= '\n') [' '..'\127']

parseMarkdownBody :: Parser [Content]
parseMarkdownBody = skipEmptyLines >> parseMany parseMarkdownContent

parseNewline :: Parser Char
parseNewline = parseChar '\n'

skipHorizontalWhitespace :: Parser ()
skipHorizontalWhitespace = parseMany (parseAnyChar " \t") >> return ()

skipEmptyLines :: Parser ()
skipEmptyLines = do
    _ <- parseMany (skipHorizontalWhitespace >> parseNewline)
    return ()

parseMarkdownContent :: Parser Content
parseMarkdownContent = do
    content <- parseSection <|> parseCodeBlock <|> parseList <|> parseParagraph
    skipEmptyLines
    return content

parseParagraph :: Parser Content
parseParagraph = do
    lines <- parseSome parseNonEmptyLine
    return $ Paragraph (parseTextElements (unlines lines))

parseNonEmptyLine :: Parser String
parseNonEmptyLine = do
    content <- parseSome (parseAnyChar nonNewlineChars)
    _ <- parseNewline
    return content
  where
    nonNewlineChars = filter (/= '\n') [' '..'\127']

parseSection :: Parser Content
parseSection = do
    level <- parseSome (parseChar '#')
    _ <- skipHorizontalWhitespace
    title <- parseMany (parseAnyChar nonNewlineChars)  
    _ <- parseNewline
    skipEmptyLines
    contents <- parseMany parseMarkdownContent
    return $ Section (Just title) contents
  where
    nonNewlineChars = filter (/= '\n') [' '..'\127']

parseCodeBlock :: Parser Content
parseCodeBlock = do
    _ <- parseString "```"
    _ <- parseNewline
    code <- parseCodeContent
    _ <- parseString "```"
    _ <- parseNewline
    return $ CodeBlock code

parseCodeContent :: Parser String
parseCodeContent = do
    lines <- parseMany parseCodeLine
    return (concat lines)

parseCodeLine :: Parser String
parseCodeLine = do
    notFollowedBy (parseString "```")
    line <- parseMany (parseAnyChar nonNewlineChars)
    _ <- parseNewline
    return (line ++ "\n")
  where
    nonNewlineChars = filter (/= '\n') [' '..'\127']

notFollowedBy :: Parser a -> Parser ()
notFollowedBy p = Parser $ \input -> case runParser p input of
    Nothing -> Just ((), input)
    Just _ -> Nothing

parseList :: Parser Content
parseList = do
    items <- parseSome parseListItem
    return $ List items

parseListItem :: Parser Item
parseListItem = do
    _ <- parseString "- "
    content <- parseMany (parseAnyChar nonNewlineChars)
    _ <- parseNewline
    return $ Item (parseTextElements content)
  where
    nonNewlineChars = filter (/= '\n') [' '..'\127']

parseTextElements :: String -> [Element]
parseTextElements text = 
    case runParser parseInlineElements text of
        Just (elements, "") -> elements
        _ -> [Text text]

parseInlineElements :: Parser [Element]
parseInlineElements = parseMany parseInlineElement

parseInlineElement :: Parser Element
parseInlineElement = 
    parseBold <|>
    parseItalic <|>
    parseCode <|>
    parseLink <|>
    parseImage <|>
    parseText

parseBold :: Parser Element
parseBold = do
    _ <- parseString "**"
    content <- parseMany (notString "**" >> parseNonNewlineChar)
    _ <- parseString "**"
    return $ Bold [Text content]

parseNonNewlineChar :: Parser Char
parseNonNewlineChar = parseAnyChar (filter (/= '\n') [' '..'\127'])

parseItalic :: Parser Element
parseItalic = do
    _ <- parseChar '*'
    notFollowedBy (parseChar '*')
    content <- parseMany (notString "*" >> parseNonNewlineChar)
    _ <- parseChar '*'
    return $ Italic [Text content]

parseCode :: Parser Element
parseCode = do
    _ <- parseChar '`'
    content <- parseMany (notString "`" >> parseNonNewlineChar)
    _ <- parseChar '`'
    return $ Code content

parseLink :: Parser Element
parseLink = do
    _ <- parseChar '['
    text <- parseMany (notString "]" >> parseNonNewlineChar)
    _ <- parseChar ']'
    _ <- parseChar '('
    url <- parseMany (notString ")" >> parseNonNewlineChar)
    _ <- parseChar ')'
    return $ Link text url

parseImage :: Parser Element
parseImage = do
    _ <- parseChar '!'
    _ <- parseChar '['
    alt <- parseMany (notString "]" >> parseNonNewlineChar)
    _ <- parseChar ']'
    _ <- parseChar '('
    url <- parseMany (notString ")" >> parseNonNewlineChar)
    _ <- parseChar ')'
    return $ Image alt url

parseText :: Parser Element
parseText = do
    text <- parseSome (
        notString "**" >>
        notString "*" >>
        notString "`" >>
        notString "[" >>
        notString "![" >>
        parseNonNewlineChar
        )
    return $ Text text

notString :: String -> Parser ()
notString s = notFollowedBy (parseString s)