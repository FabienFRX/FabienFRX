# Parser

The parser transforms GLaDOS source code into an Abstract Syntax Tree (AST).

## Overview

The parser is implemented using Parsec, a parser combinator library for Haskell. It uses a recursive descent parsing strategy.

**Location**: `src/part2/Compiler/Parser.hs`

## Parser Architecture

### Main Entry Point

```haskell
parseProgram :: String -> Either String [AST]
```

This function:
1. Takes source code as a string
2. Runs the parser
3. Returns either an error or a list of AST nodes

### Parser Combinators

The parser is built from smaller parsers combined together:

```text
Program → Function+
Function → "fn" Identifier "(" Params ")" "{" Statement* "}"
Statement → Return | Assignment | Expression
Expression → If | Comparison | Term | Factor
```

## Parsing Stages

### 1. Lexical Analysis

The parser handles whitespace and tokens:

```haskell
-- Skip whitespace
spaces

-- Parse identifiers
identifier = do
    first <- letter <|> char '_'
    rest <- many (alphaNum <|> char '_')
    return (first : rest)

-- Parse integers
integer = read <$> many1 digit
```

### 2. Expression Parsing

Expressions are parsed with proper operator precedence:

#### Precedence Levels

1. **Primary**: Literals, variables, parentheses, function calls
2. **Multiplicative**: `*`, `/`
3. **Additive**: `+`, `-`
4. **Comparison**: `<`, `>`, `<=`, `>=`
5. **Equality**: `==`, `!=`

#### Implementation

```haskell
parseExpr :: Parser AST
parseExpr = parseIf <|> parseAssign <|> parseComparison

parseComparison :: Parser AST
parseComparison = chainl1 parseAdditive comparisonOp

parseAdditive :: Parser AST
parseAdditive = chainl1 parseMultiplicative additiveOp

parseMultiplicative :: Parser AST
parseMultiplicative = chainl1 parsePrimary multiplicativeOp
```

### 3. Statement Parsing

Statements include returns, assignments, and expressions:

```haskell
parseStatement :: Parser AST
parseStatement = parseReturn <|> parseAssign <|> parseExpr

parseReturn :: Parser AST
parseReturn = do
    reserved "return"
    expr <- parseExpr
    return (AReturn expr)

parseAssign :: Parser AST
parseAssign = do
    var <- identifier
    reservedOp "="
    expr <- parseExpr
    return (AAssign var expr)
```

### 4. Function Parsing

Functions are parsed as complete units:

```haskell
parseFunction :: Parser AST
parseFunction = do
    reserved "fn"
    name <- identifier
    params <- parens (identifier `sepBy` comma)
    body <- braces (many parseStatement)
    return (AFuncDef name params (ASeq body))
```

## Operator Parsing

### Binary Operators

Binary operators are parsed using `chainl1` for left-associativity:

```haskell
additiveOp :: Parser (AST -> AST -> AST)
additiveOp =
    (reservedOp "+" >> return AAdd) <|>
    (reservedOp "-" >> return ASub)

multiplicativeOp :: Parser (AST -> AST -> AST)
multiplicativeOp =
    (reservedOp "*" >> return AMul) <|>
    (reservedOp "/" >> return ADiv)

comparisonOp :: Parser (AST -> AST -> AST)
comparisonOp =
    (reservedOp "==" >> return AEq) <|>
    (reservedOp "!=" >> return ANotEq) <|>
    (reservedOp "<=" >> return ALessEq) <|>
    (reservedOp ">=" >> return AGreaterEq) <|>
    (reservedOp "<" >> return ALess) <|>
    (reservedOp ">" >> return AGreater)
```

### Operator Precedence Table

| Level | Operators | Associativity |
|-------|-----------|---------------|
| 1 (highest) | Function calls | Left |
| 2 | Unary `-` | Right |
| 3 | `*`, `/` | Left |
| 4 | `+`, `-` | Left |
| 5 | `<`, `>`, `<=`, `>=` | Left |
| 6 (lowest) | `==`, `!=` | Left |

## Parsing Examples

### Simple Expression

Input:
```glados
2 + 3 * 4
```

Parse tree:
```
AAdd
  ├─ AInt 2
  └─ AMul
      ├─ AInt 3
      └─ AInt 4
```

### If-Else Statement

Input:
```glados
if (x > 0) {
    return 1
} else {
    return 0
}
```

Parse tree:
```
AIf
  ├─ AGreater
  │   ├─ AVar "x"
  │   └─ AInt 0
  ├─ AReturn (AInt 1)
  └─ AReturn (AInt 0)
```

### Function Definition

Input:
```glados
fn add(a, b) {
    return a + b
}
```

Parse tree:
```
AFuncDef "add" ["a", "b"]
  └─ ASeq
      └─ AReturn
          └─ AAdd
              ├─ AVar "a"
              └─ AVar "b"
```

## Error Handling

The parser provides error messages for invalid syntax:

### Unexpected Token

```
Parse error: unexpected 'x', expecting '}'
```

### Missing Operator

```
Parse error: unexpected end of input, expecting operator
```

### Invalid Expression

```
Parse error: unexpected ')', expecting expression
```

## Keywords and Reserved Words

Reserved keywords that cannot be used as identifiers:

- `fn` - Function definition
- `if` - Conditional
- `else` - Alternative branch
- `return` - Return statement

## Lexical Tokens

### Identifiers

Pattern: `[a-zA-Z_][a-zA-Z0-9_]*`

Examples:
```
x
myVar
my_function_2
_private
```

### Integers

Pattern: `-?[0-9]+`

Examples:
```
42
0
-15
1000
```

### Operators

```
+  -  *  /       // Arithmetic
== != < > <= >=  // Comparison
=                // Assignment
```

### Delimiters

```
( )    // Parentheses
{ }    // Braces
,      // Comma
;      // Semicolon (optional, implicit)
```

## Whitespace Handling

Whitespace (spaces, tabs, newlines) is ignored but serves to separate tokens:

```glados
fn main(){return 42}        // Valid

fn main() {
    return 42
}                           // Also valid (preferred)
```

## Implementation Details

### Parsec Usage

The parser uses Parsec's combinator library:

```haskell
import Text.Parsec
import Text.Parsec.String (Parser)
import Text.Parsec.Expr
import Text.Parsec.Token
import Text.Parsec.Language
```

### Token Definition

```haskell
langDef :: LanguageDef st
langDef = emptyDef
    { commentLine = ""
    , identStart = letter <|> char '_'
    , identLetter = alphaNum <|> char '_'
    , reservedNames = ["fn", "if", "else", "return"]
    , reservedOpNames = ["+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">=", "="]
    }
```

### Parser Monad

The parser runs in the Parsec monad:

```haskell
parseProgram :: String -> Either ParseError [AST]
parseProgram input = parse program "" input
```

## Future Enhancements

Potential parser improvements:

- Better error messages with line/column numbers
- Support for comments
- String literals
- Array literals
- More operators (modulo, bitwise, etc.)

## Next Steps

- Understand the [AST structure](./ast.md)
- Learn about [code generation](./codegen.md)
- See how the [compiler](./compiler.md) uses the parser
