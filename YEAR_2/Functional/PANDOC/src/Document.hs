{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-mypandoc-andi.berard [WSL: Debian]
-- File description:
-- Document
-}

module Document
    ( Document(..)
    , Header(..)
    , Content(..)
    , Element(..)
    , Item(..)
    , FormatType(..)
    ) where

data Document = Document
    { header :: Header
    , body :: [Content]
    } deriving (Show, Eq)

data Header = Header
    { title :: String
    , author :: Maybe String
    , date :: Maybe String
    } deriving (Show, Eq)

data Content = Paragraph [Element]
             | Section (Maybe String) [Content]
             | CodeBlock String
             | List [Item]
             deriving (Show, Eq)

data Item = Item [Element] deriving (Show, Eq)

data Element = Text String
             | Italic [Element]
             | Bold [Element]
             | Code String
             | Link String String
             | Image String String
             deriving (Show, Eq)

data FormatType = XML | JSON | Markdown deriving (Show, Eq)