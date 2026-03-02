{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- XMLPrinter
-}

module XMLPrinter (documentToXML) where

import Document

documentToXML :: Document -> String
documentToXML doc = 
    "<document>\n" ++
    headerToXML (header doc) ++
    bodyToXML (body doc) ++
    "</document>"

headerToXML :: Header -> String
headerToXML h = 
    "  <header title=\"" ++ title h ++ "\"" ++
    maybe "" (\a -> " author=\"" ++ a ++ "\"") (author h) ++
    maybe "" (\d -> " date=\"" ++ d ++ "\"") (date h) ++
    "></header>\n"

bodyToXML :: [Content] -> String
bodyToXML contents =
    "  <body>\n" ++
    concatMap contentToXML contents ++
    "  </body>\n"

contentToXML :: Content -> String
contentToXML (Paragraph elements) =
    "    <paragraph>" ++ concatMap elementToXML elements ++ "</paragraph>\n"
contentToXML (Section titleOpt contents) =
    "    <section" ++
    maybe "" (\t -> " title=\"" ++ t ++ "\"") titleOpt ++
    ">\n" ++
    concatMap contentToXML contents ++
    "    </section>\n"
contentToXML (CodeBlock code) =
    "    <codeblock>" ++ escape code ++ "</codeblock>\n"
contentToXML (List items) =
    "    <list>\n" ++
    concatMap itemToXML items ++
    "    </list>\n"

elementToXML :: Element -> String
elementToXML (Text text) = escape text
elementToXML (Italic elements) =
    "<italic>" ++ concatMap elementToXML elements ++ "</italic>"
elementToXML (Bold elements) =
    "<bold>" ++ concatMap elementToXML elements ++ "</bold>"
elementToXML (Code code) =
    "<code>" ++ escape code ++ "</code>"
elementToXML (Link text url) =
    "<link url=\"" ++ escape url ++ "\">" ++ escape text ++ "</link>"
elementToXML (Image alt url) =
    "<image url=\"" ++ escape url ++ "\" alt=\"" ++ escape alt ++ "\"/>"

itemToXML :: Item -> String
itemToXML (Item elements) =
    "<item>" ++ concatMap elementToXML elements ++ "</item>\n"

escape :: String -> String
escape = concatMap escapeChar
  where
    escapeChar '&' = "&amp;"
    escapeChar '<' = "&lt;"
    escapeChar '>' = "&gt;"
    escapeChar '"' = "&quot;"
    escapeChar '\'' = "&apos;"
    escapeChar c = [c]