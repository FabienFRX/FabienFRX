{-
-- EPITECH PROJECT, 2025
-- my_pandoc
-- File description:
-- Test Suite
-}

module Main (main) where

import Test.Hspec
import Parser
import Document
import XMLParser (parseXMLDocument)
import JSONParser (parseJSONDocument)
import MarkdownParser (parseMarkdownDocument)
import XMLPrinter (documentToXML)
import JSONPrinter (documentToJSON)
import MarkdownPrinter (documentToMarkdown)

main :: IO ()
main = hspec $ do
  describe "Parser Combinators" $ do
    it "parseChar parses a single character" $ do
      runParser (parseChar 'a') "abc" `shouldBe` Just ('a', "bc")
      runParser (parseChar 'x') "abc" `shouldBe` Nothing

    it "parseAnyChar parses any character from a set" $ do
      runParser (parseAnyChar "abc") "bcd" `shouldBe` Just ('b', "cd")
      runParser (parseAnyChar "xyz") "abc" `shouldBe` Nothing

    it "parseOr tries alternatives" $ do
      runParser (parseChar 'a' `parseOr` parseChar 'b') "abc" `shouldBe` Just ('a', "bc")
      runParser (parseChar 'x' `parseOr` parseChar 'b') "bcd" `shouldBe` Just ('b', "cd")
      runParser (parseChar 'x' `parseOr` parseChar 'y') "abc" `shouldBe` Nothing

    it "parseAnd combines two parsers" $ do
      runParser (parseAnd (parseChar 'a') (parseChar 'b')) "abc" `shouldBe` Just (('a', 'b'), "c")
      runParser (parseAnd (parseChar 'a') (parseChar 'x')) "abc" `shouldBe` Nothing

    it "parseMany parses zero or more occurrences" $ do
      runParser (parseMany (parseChar 'a')) "aaabcd" `shouldBe` Just ("aaa", "bcd")
      runParser (parseMany (parseChar 'a')) "bcd" `shouldBe` Just ("", "bcd")

    it "parseSome parses one or more occurrences" $ do
      runParser (parseSome (parseChar 'a')) "aaabcd" `shouldBe` Just ("aaa", "bcd")
      runParser (parseSome (parseChar 'a')) "bcd" `shouldBe` Nothing

    it "parseUInt parses unsigned integers" $ do
      runParser parseUInt "123abc" `shouldBe` Just (123, "abc")
      runParser parseUInt "abc123" `shouldBe` Nothing

    it "parseInt parses signed integers" $ do
      runParser parseInt "123abc" `shouldBe` Just (123, "abc")
      runParser parseInt "-123abc" `shouldBe` Just (-123, "abc")
      runParser parseInt "abc123" `shouldBe` Nothing

  describe "Simple Document Parsing" $ do
    it "can parse a simple XML document" $ do
      let input = "<document>\n  <header title=\"Simple example\"></header>\n  <body>\n    <paragraph>This is a simple example</paragraph>\n  </body>\n</document>"
      let expected = Document (Header "Simple example" Nothing Nothing) [Paragraph [Text "This is a simple example"]]
      case runParser parseXMLDocument input of
        Just (doc, _) -> doc `shouldBe` expected
        Nothing -> expectationFailure "Failed to parse XML document"

    it "can parse a simple JSON document" $ do
      let input = "{\"header\":{\"title\":\"Simple example\"},\"body\":[[\"This is a simple example\"]]}"
      let expected = Document (Header "Simple example" Nothing Nothing) [Paragraph [Text "This is a simple example"]]
      case runParser parseJSONDocument input of
        Just (doc, _) -> doc `shouldBe` expected
        Nothing -> expectationFailure "Failed to parse JSON document"

    it "can parse a simple Markdown document" $ do
      let input = "---\ntitle: Simple example\n---\n\nThis is a simple example\n"
      let expected = Document (Header "Simple example" Nothing Nothing) [Paragraph [Text "This is a simple example\n"]]
      case runParser parseMarkdownDocument input of
        Just (doc, _) -> doc `shouldBe` expected
        Nothing -> expectationFailure "Failed to parse Markdown document"

  describe "Document Printing" $ do
    it "can convert Document to XML" $ do
      let doc = Document (Header "Simple example" Nothing Nothing) [Paragraph [Text "This is a simple example"]]
      let xml = documentToXML doc
      xml `shouldContain` "<document>"
      xml `shouldContain` "<header title=\"Simple example\""
      xml `shouldContain` "<paragraph>This is a simple example</paragraph>"

    it "can convert Document to JSON" $ do
      let doc = Document (Header "Simple example" Nothing Nothing) [Paragraph [Text "This is a simple example"]]
      let json = documentToJSON doc
      json `shouldContain` "\"header\":"
      json `shouldContain` "\"title\": \"Simple example\""
      json `shouldContain` "\"body\": [\n    [\"This is a simple example\"]\n  ]"

    it "can convert Document to Markdown" $ do
      let doc = Document (Header "Simple example" Nothing Nothing) [Paragraph [Text "This is a simple example"]]
      let markdown = documentToMarkdown doc
      markdown `shouldContain` "---"
      markdown `shouldContain` "title: Simple example"
      markdown `shouldContain` "This is a simple example"

  describe "Complex Document Elements" $ do
    it "can parse and print formatted text" $ do
      let doc = Document (Header "Formatting Test" Nothing Nothing) 
                [Paragraph [Text "Normal ", Bold [Text "bold"], Text " and ", Italic [Text "italic"], Text " text"]]
      
      -- Test XML output
      let xml = documentToXML doc
      xml `shouldContain` "<bold>bold</bold>"
      xml `shouldContain` "<italic>italic</italic>"
      
      -- Test JSON output
      let json = documentToJSON doc
      json `shouldContain` "{\"bold\":\"bold\"}"
      json `shouldContain` "{\"italic\":\"italic\"}"

    it "can parse and print links and images" $ do
      let doc = Document (Header "Links Test" Nothing Nothing) 
                [Paragraph [Text "Check this ", Link "link" "https://example.com", Text " and ", Image "picture" "image.jpg"]]
      
      -- Test XML output
      let xml = documentToXML doc
      xml `shouldContain` "<link url=\"https://example.com\">link</link>"
      xml `shouldContain` "<image url=\"image.jpg\" alt=\"picture\"/>"
      
      -- Test JSON output
      let json = documentToJSON doc
      json `shouldContain` "\"link\":{\"content\":[\"link\"],\"url\":\"https://example.com\"}"
      json `shouldContain` "\"image\":{\"alt\":[\"picture\"],\"url\":\"image.jpg\"}"

    it "can parse and print lists" $ do
      let doc = Document (Header "List Test" Nothing Nothing) 
                [List [Item [Text "Item 1"], Item [Text "Item 2"], Item [Bold [Text "Item 3"]]]]
      
      -- Test XML output
      let xml = documentToXML doc
      xml `shouldContain` "<list>"
      xml `shouldContain` "<item>Item 1</item>"
      xml `shouldContain` "<item>Item 2</item>"
      xml `shouldContain` "<item><bold>Item 3</bold></item>"
      
      -- Test JSON output
      let json = documentToJSON doc
      json `shouldContain` "\"list\":[[\"Item 1\"],[\"Item 2\"],[{\"bold\":\"Item 3\"}]]"