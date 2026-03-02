{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- MarkdownPrinter
-}

module MarkdownPrinter (documentToMarkdown) where

import Document

documentToMarkdown :: Document -> String
documentToMarkdown doc =
    headerToMarkdown (header doc) ++
    "\n\n" ++
    bodyToMarkdown (body doc)

headerToMarkdown :: Header -> String
headerToMarkdown h =
    "---\n" ++
    "title: " ++ title h ++ "\n" ++
    maybe "" (\a -> "author: " ++ a ++ "\n") (author h) ++
    maybe "" (\d -> "date: " ++ d ++ "\n") (date h) ++
    "---"

bodyToMarkdown :: [Content] -> String
bodyToMarkdown contents =
    concatMap contentToMarkdown contents

contentToMarkdown :: Content -> String
contentToMarkdown (Paragraph elements) =
    concatMap elementToMarkdown elements ++ "\n\n"
contentToMarkdown (Section (Just t) contents) =
    "# " ++ t ++ "\n\n" ++
    concatMap contentToMarkdown contents
contentToMarkdown (Section Nothing contents) =
    concatMap contentToMarkdown contents
contentToMarkdown (CodeBlock code) =
    "```\n" ++ code ++ "\n```\n\n"
contentToMarkdown (List items) =
    concatMap itemToMarkdown items ++ "\n"

elementToMarkdown :: Element -> String
elementToMarkdown (Text text) = text
elementToMarkdown (Italic elements) =
    "*" ++ concatMap elementToMarkdown elements ++ "*"
elementToMarkdown (Bold elements) =
    "**" ++ concatMap elementToMarkdown elements ++ "**"
elementToMarkdown (Code code) =
    "`" ++ code ++ "`"
elementToMarkdown (Link text url) =
    "[" ++ text ++ "](" ++ url ++ ")"
elementToMarkdown (Image alt url) =
    "![" ++ alt ++ "](" ++ url ++ ")"

itemToMarkdown :: Item -> String
itemToMarkdown (Item elements) =
    "- " ++ concatMap elementToMarkdown elements ++ "\n"