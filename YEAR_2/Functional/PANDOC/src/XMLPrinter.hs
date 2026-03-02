{-
-- EPITECH PROJECT, 2025
-- PANDOC
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
headerToXML h = buildHeaderTag (title h) (author h) (date h)

buildHeaderTag :: String -> Maybe String -> Maybe String -> String
buildHeaderTag t a d = 
    "  <header title=\"" ++ escape t ++ "\"" ++
    renderAuthor a ++
    renderDate d ++
    renderClosingTag a d

renderAuthor :: Maybe String -> String
renderAuthor Nothing = ">"
renderAuthor (Just a) = ">\n    <author>" ++ escape a ++ "</author>"

renderDate :: Maybe String -> String
renderDate Nothing = ""
renderDate (Just d) = "\n    <date>" ++ escape d ++ "</date>"

renderClosingTag :: Maybe String -> Maybe String -> String
renderClosingTag Nothing Nothing = "</header>\n"
renderClosingTag _ _ = "\n  </header>\n"

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
    maybe ">" (\t -> " title=\"" ++ escape t ++ "\">") titleOpt ++
    (if null contents then "</section>\n" else "\n" ++
    concatMap contentToXML contents ++
    "    </section>\n")
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
    "<image url=\"" ++ escape url ++ "\">" ++ escape alt ++ "</image>"

itemToXML :: Item -> String
itemToXML (Item elements) =
    "      <paragraph>" ++ concatMap elementToXML elements ++ "</paragraph>\n"

escape :: String -> String
escape = concatMap escapeChar
  where
    escapeChar '&' = "&amp;"
    escapeChar '<' = "&lt;"
    escapeChar '>' = "&gt;"
    escapeChar '"' = "&quot;"
    escapeChar '\'' = "&apos;"
    escapeChar c = [c]