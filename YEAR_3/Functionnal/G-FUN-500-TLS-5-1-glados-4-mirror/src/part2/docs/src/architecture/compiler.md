# Compiler Architecture

The GLaDOS compiler transforms source code into bytecode that can be executed by the virtual machine.

## Overview

The compilation process follows these stages:

```text
Source Code (.gld)
       ↓
  [Lexer/Parser] → Tokens
       ↓
   [AST Builder] → Abstract Syntax Tree
       ↓
 [Code Generator] → Bytecode Instructions
       ↓
   [Serializer] → Bytecode File (.bc)
```

## Compiler Pipeline

### 1. Parsing

The parser (`Compiler/Parser.hs`) uses the Parsec library to transform source code into an Abstract Syntax Tree (AST).

**Location**: `src/part2/Compiler/Parser.hs`

#### Key Functions

```haskell
parseProgram :: String -> Either String [AST]
```

Parses a complete GLaDOS program and returns a list of AST nodes (function definitions).

#### Example

Input:
```glados
fn main() {
    return 42
}
```

Output (conceptual):
```haskell
[AFuncDef "main" [] (AReturn (AInt 42))]
```

### 2. AST Representation

The AST (`Compiler/AST.hs`) represents the program structure in a tree form.

**Location**: `src/part2/Compiler/AST.hs`

#### AST Data Type

```haskell
data AST =
    AInt Int                          -- Integer literal
    | ABool Bool                      -- Boolean literal
    | AVar String                     -- Variable reference
    | AAssign String AST              -- Assignment
    | AAdd AST AST                    -- Addition
    | ASub AST AST                    -- Subtraction
    | AMul AST AST                    -- Multiplication
    | ADiv AST AST                    -- Division
    | AEq AST AST                     -- Equality
    | ANotEq AST AST                  -- Inequality
    | ALess AST AST                   -- Less than
    | AGreater AST AST                -- Greater than
    | ALessEq AST AST                 -- Less or equal
    | AGreaterEq AST AST              -- Greater or equal
    | AIf AST AST AST                 -- If-else
    | AFuncDef String [String] AST    -- Function definition
    | ACall String [AST]              -- Function call
    | AReturn AST                     -- Return statement
    | ASeq [AST]                      -- Statement sequence
```

### 3. Code Generation

The compiler (`Compiler/Compiler.hs`) translates the AST into bytecode instructions.

**Location**: `src/part2/Compiler/Compiler.hs`

#### Main Compilation Function

```haskell
compile :: [AST] -> Either String ([Instruction], Map.Map String ([String], [Instruction]))
```

This function:
1. Separates function definitions from top-level code
2. Verifies that `main()` exists and has no parameters
3. Compiles each function to bytecode
4. Returns the entry point code and a map of function bytecode

#### Compilation Rules

**Literals**:
```haskell
AInt n    → [Push (VInt n)]
ABool b   → [Push (VBool b)]
```

**Variables**:
```haskell
AVar name        → [LoadVar name]
AAssign name e   → compile(e) ++ [StoreVar name]
```

**Binary Operations**:
```haskell
ABinOp l r → compile(l) ++ compile(r) ++ [BinOp op]
```

**Conditionals**:
```haskell
AIf cond then else →
    compile(cond) ++
    [JumpIfFalse thenLen+1] ++
    compile(then) ++
    [Jump elseLen] ++
    compile(else)
```

**Function Calls**:
```haskell
ACall name args →
    compile(args[0]) ++ ... ++ compile(args[n]) ++
    [CallFunc name argCount]
```

**Return**:
```haskell
AReturn e → compile(e) ++ [Return]
```

### 4. Validation

The compiler performs several validations:

#### Entry Point Check

```haskell
case Map.lookup "main" funcMap of
    Nothing -> Left "Entry point 'main()' function not found"
    Just (params, _) ->
        if not (null params)
            then Left "main() function must have no parameters"
            else return ()
```

#### Top-Level Code Check

```haskell
if not (null topLevel)
    then Left "Top-level code is not allowed. All code must be inside functions."
    else return ()
```

## Bytecode Format

### Instruction Set

The bytecode uses a stack-based instruction set:

| Instruction | Description |
|-------------|-------------|
| `Push Value` | Push a value onto the stack |
| `Pop` | Pop a value from the stack |
| `LoadVar String` | Load variable onto stack |
| `StoreVar String` | Store top of stack in variable |
| `BinOp Op` | Binary operation on top two stack values |
| `JumpIfFalse Int` | Conditional jump |
| `Jump Int` | Unconditional jump |
| `CallFunc String Int` | Call function with N arguments |
| `Return` | Return from function |

### Example Compilation

Source:
```glados
fn add(a, b) {
    return a + b
}

fn main() {
    result = add(10, 20)
    return result
}
```

Bytecode (conceptual):
```
main:
    [CallFunc "main" 0, Return]

functions:
    add(a, b):
        [LoadVar "a", LoadVar "b", BinOp Add, Return]

    main():
        [Push (VInt 10), Push (VInt 20), CallFunc "add" 2,
         StoreVar "result", LoadVar "result", Return]
```

## Binary Serialization

The compiled bytecode is serialized to a binary file using Haskell's `Binary` library.

**Location**: `src/part2/VM/CompilerInterface.hs`

### Bytecode Structure

```haskell
data Bytecode = Bytecode
    { entryPoint :: [Instruction]
    , functions :: Map.Map String ([String], [Instruction])
    }
```

### Serialization

```haskell
saveBytecode :: FilePath -> Bytecode -> IO ()
```

Saves bytecode to a `.bc` file in binary format.

## Compiler Usage

### Command Line

```bash
glados-compiler [OPTIONS] <input-file>

Options:
  -o <output>    Specify output file
  -h, --help     Show help message
```

### Examples

```bash
# Compile to default output
./glados-compiler program.gld

# Compile with custom output
./glados-compiler -o program.bc program.gld
```

## Error Handling

The compiler provides detailed error messages:

### Syntax Errors

```
Parse error at line 5: unexpected 'x', expecting '}'
```

### Semantic Errors

```
Error: Entry point 'main()' function not found
Error: main() function must have no parameters
Error: Top-level code is not allowed. All code must be inside functions.
Error: Variable not found: x
```

## Optimization

The compiler currently performs minimal optimization. Future optimizations could include:

- Constant folding
- Dead code elimination
- Tail call optimization
- Register allocation

## Implementation Details

### Parser Combinators

The parser uses Parsec combinators to build a recursive descent parser:

```haskell
parseExpr = parseIf <|> parseAssign <|> parseComparison

parseComparison = chainl1 parseAdditive comparisonOp

parseAdditive = chainl1 parseMultiplicative additiveOp
```

### Instruction Generation

Instructions are generated using a simple recursive traversal:

```haskell
compileAST :: AST -> Either String [Instruction]
compileAST (AInt n) = Right [Push (VInt n)]
compileAST (AAdd l r) = do
    leftCode <- compileAST l
    rightCode <- compileAST r
    return $ leftCode ++ rightCode ++ [BinOp Add]
```

### Function Table

Functions are compiled into a map for fast lookup:

```haskell
type FunctionMap = Map.Map String ([String], [Instruction])
```

Each entry contains:
- Parameter names (for debugging)
- Compiled bytecode instructions

## Next Steps

- Understand the [AST structure](./ast.md) in detail
- Learn about [code generation](./codegen.md) techniques
- Explore the [Virtual Machine](./vm.md) that executes the bytecode
- See the [bytecode format](./bytecode.md) specification
