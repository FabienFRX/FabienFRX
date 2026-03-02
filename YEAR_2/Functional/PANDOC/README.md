# 📄 MyPandoc - Document Converter

```ascii

   _____          __________                     .___               
  /     \  ___.__.\______   \_____     ____    __| _/ ____    ____  
 /  \ /  \<   |  | |     ___/\__  \   /    \  / __ | /  _ \ _/ ___\ 
/    Y    \\___  | |    |     / __ \_|   |  \/ /_/ |(  <_> )\  \___ 
\____|__  // ____| |____|    (____  /|___|  /\____ | \____/  \___  >
        \/ \/                     \/      \/      \/             \/ 

```

A lightweight document converter inspired by Pandoc, built in Haskell.

## 🔍 Overview

MyPandoc is a document conversion tool that transforms documents between XML, JSON, and Markdown formats. It implements a custom recursive descent parser library for handling various document formats, maintaining the document structure and formatting across conversions.

## ✨ Features

- **Multiple Format Support**:
  - XML input/output
  - JSON input/output
  - Markdown input/output

- **Document Element Support**:
  - Headers with title, author, and date
  - Paragraphs and sections
  - Text formatting (bold, italic, code)
  - Links and images
  - Lists
  - Code blocks

- **Custom Parser Library**:
  - Recursive descent parsing
  - Functor, Applicative, Alternative, and Monad instances
  - Parser combinators for efficient parsing

## 🛠️ Installation

### Requirements

- GHC 9.4 or later
- Stack 2.1.3 or later

### Building from Source

```bash
# Clone the repository
    git clone https://github.com/EpitechPromo2028/B-FUN-400-TLS-4-1-mypandoc-andi.berard.git
    cd B-FUN-400-TLS-4-1-mypandoc-andi.berard

# Build using make
    make

# Or build directly with stack
    stack build
```

## 📋 Usage

```
USAGE: ./mypandoc -i ifile -f oformat [-o ofile] [-e iformat]
    ifile     path to the file to convert
    oformat   output format (xml, json, markdown)
    ofile     path to the output file
    iformat   input format (xml, json, markdown)
```

### Examples

Convert from XML to Markdown:
```bash
./mypandoc -i example.xml -f markdown
```

Convert from JSON to XML with explicit output file:
```bash
./mypandoc -i document.json -f xml -o output.xml
```

Convert with explicit input format:
```bash
./mypandoc -i document.json -e json -f markdown -o output.md
```

## 📂 Project Structure

- `src/` - Source code directory
  - `Main.hs` - Main program entry point
  - `Document.hs` - Document data types
  - `Parser.hs` - Custom parser library
  - `XMLParser.hs` - XML format parser
  - `JSONParser.hs` - JSON format parser
  - `MarkdownParser.hs` - Markdown format parser
  - `XMLPrinter.hs` - XML format printer
  - `JSONPrinter.hs` - JSON format printer
  - `MarkdownPrinter.hs` - Markdown format printer
- `FileTypes/` - Example files for testing
  - `xml/` - XML example files
    - `simple.xml` - Simple XML document
    - `formatting.xml` - XML with formatting
    - `complex.xml` - Complex XML document
  - `json/` - JSON example files
    - `simple.json` - Simple JSON document
    - `formatting.json` - JSON with formatting
    - `complex.json` - Complex JSON document
  - `markdown/` - Markdown example files
    - `simple.md` - Simple Markdown document
    - `formatting.md` - Markdown with formatting
    - `complex.md` - Complex Markdown document
- `test/` - Test suite directory
  - `Spec.hs` - Test specifications

## 🧪 Testing

Run the test suite:

```bash
# Build using make
    make test_rule

# Or build directly with stack
    stack test
```

Run the comprehensive test script:

```bash
# After compilation, run:
    ./test_syntax.sh
```

## 🔧 Implementation Details

MyPandoc implements a custom document model that can represent structured documents with formatting. The project uses a custom-built parsing library based on functional combinators, implementing:

- Character and string parsers
- Higher-order parser combinators
- Number parsers
- White space management

The implementation follows functional programming principles with immutable data structures and type classes.

## 🙏 Acknowledgments

- Inspired by [Pandoc](https://pandoc.org/) by John MacFarlane
- Project structure based on Stack templates

## 👨‍💻 Authors

- [Andi Berdard](https://github.com/Andiberard)
- [Marceau Kersuzan](https://github.com/Andiberard)
- [Fabien Fraixanet](https://github.com/FabienFRX)
- [Gobijan Nathakrishnan](https://github.com/GobyNathan)

## 📜 License

This project is licensed under the MIT License.
