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
    "  \"header\": " ++ headerToJSON (header doc) ++ ",\n" ++
    "  \"body\": " ++ bodyToJSON (body doc) ++ "\n" ++
    "}"

headerToJSON :: Header -> String
headerToJSON h =
    "{\n" ++
    "    \"title\": \"" ++ title h ++ "\"" ++
    maybe "" (\a -> ",\n    \"author\": \"" ++ a ++ "\"") (author h) ++
    maybe "" (\d -> ",\n    \"date\": \"" ++ d ++ "\"") (date h) ++
    "\n  }"

bodyToJSON :: [Content] -> String
bodyToJSON contents =
    "[\n" ++
    intercalateContent (map contentToJSON contents) ++
    "\n  ]"

contentToJSON :: Content -> String
contentToJSON (Paragraph elements) =
    "    {\n" ++
    "      \"type\": \"paragraph\",\n" ++
    "      \"content\": [\n" ++ 
    intercalateElement (map elementToJSON elements) ++ 
    "\n      ]\n" ++
    "    }"
contentToJSON (Section titleOpt contents) =
    "    {\n" ++
    "      \"type\": \"section\",\n" ++
    maybe "" (\t -> "      \"title\": \"" ++ t ++ "\",\n") titleOpt ++
    "      \"content\": [\n" ++ 
    intercalateContent (map contentToJSON contents) ++ 
    "\n      ]\n" ++
    "    }"
contentToJSON (CodeBlock code) =
    "    {\n" ++
    "      \"type\": \"codeblock\",\n" ++
    "      \"content\": \"" ++ code ++ "\"\n" ++
    "    }"
contentToJSON (List items) =
    "    {\n" ++
    "      \"type\": \"list\",\n" ++
    "      \"items\": [\n" ++ 
    intercalateItem (map itemToJSON items) ++ 
    "\n      ]\n" ++
    "    }"

elementToJSON :: Element -> String
elementToJSON (Text text) =
    "        {\"type\": \"text\", \"content\": \"" ++ text ++ "\"}"
elementToJSON (Italic elements) =
    "        {\"type\": \"italic\", \"content\": [\n" ++ 
    intercalateNestedElement (map elementToJSON elements) ++ 
    "\n        ]}"
elementToJSON (Bold elements) =
    "        {\"type\": \"bold\", \"content\": [\n" ++ 
    intercalateNestedElement (map elementToJSON elements) ++ 
    "\n        ]}"
elementToJSON (Code code) =
    "        {\"type\": \"code\", \"content\": \"" ++ code ++ "\"}"
elementToJSON (Link text url) =
    "        {\"type\": \"link\", \"text\": \"" ++ text ++ 
    "\", \"url\": \"" ++ url ++ "\"}"
elementToJSON (Image alt url) =
    "        {\"type\": \"image\", \"alt\": \"" ++ alt ++ 
    "\", \"url\": \"" ++ url ++ "\"}"

itemToJSON :: Item -> String
itemToJSON (Item elements) =
    "        {\"content\": [\n" ++ 
    intercalateNestedElement (map elementToJSON elements) ++ 
    "\n        ]}"

intercalateContent :: [String] -> String
intercalateContent [] = ""
intercalateContent [x] = x
intercalateContent (x:xs) = x ++ ",\n" ++ intercalateContent xs

intercalateElement :: [String] -> String
intercalateElement [] = ""
intercalateElement [x] = x
intercalateElement (x:xs) = x ++ ",\n" ++ intercalateElement xs

intercalateNestedElement :: [String] -> String
intercalateNestedElement [] = ""
intercalateNestedElement [x] = "          " ++ x
intercalateNestedElement (x:xs) = "          " ++ x ++ ",\n" ++ intercalateNestedElement xs

intercalateItem :: [String] -> String
intercalateItem [] = ""
intercalateItem [x] = x
intercalateItem (x:xs) = x ++ ",\n" ++ intercalateItem xs