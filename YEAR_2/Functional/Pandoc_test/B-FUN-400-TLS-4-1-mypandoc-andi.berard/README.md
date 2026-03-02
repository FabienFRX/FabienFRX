# MyPandoc

MyPandoc is a simplified version of Pandoc for document conversion between different formats. The program takes a document as input and outputs the document in another format.

## Supported Formats

- XML
- JSON
- Markdown

## Project Structure

```
mypandoc/
│
├── src/                  # All source code
│   ├── Main.hs           # Main application code
│   ├── Document.hs       # Document data model
│   ├── Parser.hs         # Parsing library
│   ├── XMLParser.hs      # XML parser
│   ├── XMLPrinter.hs     # XML printer
│   ├── JSONParser.hs     # JSON parser
│   ├── JSONPrinter.hs    # JSON printer
│   ├── MarkdownParser.hs # Markdown parser
│   └── MarkdownPrinter.hs# Markdown printer
│
├── test/                 # Test code
│   └── Spec.hs           # Test specifications
│
├── examples/             # Example documents
│
├── package.yaml         # Stack configuration
├── stack.yaml           # Stack configuration
└── Makefile             # Build system
```

## Usage

```
USAGE: ./mypandoc -i ifile -f oformat [-o ofile] [-e iformat]
    ifile    path to the file to convert
    oformat  output format (xml, json, markdown)
    ofile    path to the output file
    iformat  input format (xml, json, markdown)
```

### Examples

To convert an XML file to Markdown:
```
./mypandoc -i example.xml -f markdown
```

To convert a JSON file to XML and save to output.xml:
```
./mypandoc -i example.json -f xml -o output.xml
```

To convert a Markdown file to JSON with explicit input format:
```
./mypandoc -i example.md -f json -e markdown
```

## Building

To build the project:
```
make
```

To clean all build files:
```
make clean
```

To completely rebuild the project:
```
make re
```

## Design

The project is based on a custom parsing library inspired by parser combinators. The document model represents a structured document with headers, paragraphs, sections, and various formatting elements.

### Document Structure

- Header: title, author (optional), date (optional)
- Content: paragraphs, sections, code blocks, lists
- Elements: text, italic, bold, code, links, images

### Parsing Library

The Parser module implements a recursive descent parser with combinators. Key functions include:

- parseChar: Parse a specific character
- parseAnyChar: Parse any character from a given set
- parseMany: Parse zero or more occurrences
- parseSome: Parse one or more occurrences
- parseString: Parse a specific string

## Testing

Run tests with:
```
make test_rule
```