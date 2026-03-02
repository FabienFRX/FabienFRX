{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- JSONPrinter
-}

module JSONPrinter (documentToJSON) where

import Document

documentToJSON :: Document -> String
documentToJSON doc =
    "{\n" ++
    "    \"header\": " ++ headerToJSON (header doc) ++ ",\n" ++
    "    \"body\": " ++ bodyToJSON (body doc) ++ "\n" ++
    "}"

headerToJSON :: Header -> String
headerToJSON h = formatHeader (title h) (author h) (date h)

formatHeader :: String -> Maybe String -> Maybe String -> String
formatHeader titleText authorMaybe dateMaybe =
    "{\n" ++
    formatTitle titleText ++
    formatOptionalFields authorMaybe dateMaybe ++
    "\n    }"

formatTitle :: String -> String
formatTitle titleText = "        \"title\": \"" ++ escapeJSON titleText ++ "\""

formatOptionalFields :: Maybe String -> Maybe String -> String
formatOptionalFields authorMaybe dateMaybe =
    formatAuthor authorMaybe ++ formatDate dateMaybe
  where
    formatAuthor = maybeField "author"
    formatDate = maybeField "date"
    
    maybeField :: String -> Maybe String -> String
    maybeField _ Nothing = ""
    maybeField fieldName (Just value) = 
        ",\n        \"" ++ fieldName ++ "\": \"" ++ escapeJSON value ++ "\""

bodyToJSON :: [Content] -> String
bodyToJSON contents =
    "[\n" ++
    intercalateContent "        " contents ++
    "\n    ]"

contentToJSON :: String -> Content -> String
contentToJSON indent (Paragraph elements) =
    indent ++ "[\n" ++
    intercalateElement (indent ++ "    ") elements ++
    "\n" ++ indent ++ "]"
contentToJSON indent (Section titleOpt contents) =
    indent ++ "{\n" ++
    indent ++ "    \"section\": {\n" ++
    indent ++ "        \"title\": \"" ++ 
        escapeJSON (maybe "" id titleOpt) ++ "\",\n" ++
    indent ++ "        \"content\": [\n" ++
    intercalateContent (indent ++ "            ") contents ++
    "\n" ++ indent ++ "        ]\n" ++
    indent ++ "    }\n" ++
    indent ++ "}"
contentToJSON indent (CodeBlock code) =
    indent ++ "{\n" ++
    indent ++ "    \"codeblock\": [\n" ++
    indent ++ "        \"" ++ 
        escapeJSON (trimTrailingNewline code) ++ "\"\n" ++
    indent ++ "    ]\n" ++
    indent ++ "}"
contentToJSON indent (List items) =
    indent ++ "{\n" ++
    indent ++ "    \"list\": [\n" ++
    intercalateItems (indent ++ "        ") items ++
    "\n" ++ indent ++ "    ]\n" ++
    indent ++ "}"

elementToJSON :: String -> Element -> String
elementToJSON indent (Text text) =
    indent ++ "\"" ++ escapeJSON text ++ "\""
elementToJSON indent (Bold elements) =
    indent ++ "{\n" ++
    indent ++ "    \"bold\": \"" ++ 
        escapeJSON (extractText elements) ++ "\"\n" ++
    indent ++ "}"
elementToJSON indent (Italic elements) =
    indent ++ "{\n" ++
    indent ++ "    \"italic\": \"" ++ 
        escapeJSON (extractText elements) ++ "\"\n" ++
    indent ++ "}"
elementToJSON indent (Code code) =
    indent ++ "{\n" ++
    indent ++ "    \"code\": \"" ++ escapeJSON code ++ "\"\n" ++
    indent ++ "}"
elementToJSON indent (Link text url) =
    indent ++ "{\n" ++
    indent ++ "    \"link\": {\n" ++
    indent ++ "        \"url\": \"" ++ escapeJSON url ++ "\",\n" ++
    indent ++ "        \"content\": [\n" ++
    indent ++ "            \"" ++ escapeJSON text ++ "\"\n" ++
    indent ++ "        ]\n" ++
    indent ++ "    }\n" ++
    indent ++ "}"
elementToJSON indent (Image alt url) =
    indent ++ "{\n" ++
    indent ++ "    \"image\": {\n" ++
    indent ++ "        \"url\": \"" ++ escapeJSON url ++ "\",\n" ++
    indent ++ "        \"alt\": [\n" ++
    indent ++ "            \"" ++ escapeJSON alt ++ "\"\n" ++
    indent ++ "        ]\n" ++
    indent ++ "    }\n" ++
    indent ++ "}"

itemToJSON :: String -> Item -> String
itemToJSON indent (Item elements) =
    indent ++ "[\n" ++
    intercalateElement (indent ++ "    ") elements ++
    "\n" ++ indent ++ "]"

extractText :: [Element] -> String
extractText [] = ""
extractText (Text t:rest) = t ++ extractText rest
extractText (_:rest) = extractText rest

escapeJSON :: String -> String
escapeJSON = concatMap escapeChar
  where
    escapeChar '"' = "\\\""
    escapeChar '\\' = "\\\\"
    escapeChar '\n' = "\\n"
    escapeChar '\r' = "\\r"
    escapeChar '\t' = "\\t"
    escapeChar c
      | c < ' ' = ""
      | otherwise = [c]

trimTrailingNewline :: String -> String
trimTrailingNewline s = 
    case reverse s of
        '\n':rest -> reverse rest
        _ -> s

intercalateContent :: String -> [Content] -> String
intercalateContent _ [] = ""
intercalateContent indent [x] = contentToJSON indent x
intercalateContent indent (x:xs) = 
    contentToJSON indent x ++ ",\n" ++ intercalateContent indent xs

intercalateElement :: String -> [Element] -> String
intercalateElement _ [] = ""
intercalateElement indent [x] = elementToJSON indent x
intercalateElement indent (x:xs) = 
    elementToJSON indent x ++ ",\n" ++ intercalateElement indent xs

intercalateItems :: String -> [Item] -> String
intercalateItems _ [] = ""
intercalateItems indent [x] = itemToJSON indent x
intercalateItems indent (x:xs) = 
    itemToJSON indent x ++ ",\n" ++ intercalateItems indent xs