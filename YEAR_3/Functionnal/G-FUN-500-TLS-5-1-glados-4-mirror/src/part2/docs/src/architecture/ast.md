# Abstract Syntax Tree (AST)

The Abstract Syntax Tree represents the structure of a GLaDOS program in a tree form that's easy to analyze and compile.

## Overview

The AST is defined in `src/part2/Compiler/AST.hs` as a Haskell algebraic data type.

## AST Data Type

```haskell
data AST =
    -- Literals
    AInt Int                          -- Integer literal: 42
    | ABool Bool                      -- Boolean literal: true/false

    -- Variables
    | AVar String                     -- Variable reference: x
    | AAssign String AST              -- Assignment: x = expr

    -- Arithmetic Operators
    | AAdd AST AST                    -- Addition: x + y
    | ASub AST AST                    -- Subtraction: x - y
    | AMul AST AST                    -- Multiplication: x * y
    | ADiv AST AST                    -- Division: x / y

    -- Comparison Operators
    | AEq AST AST                     -- Equal: x == y
    | ANotEq AST AST                  -- Not equal: x != y
    | ALess AST AST                   -- Less than: x < y
    | AGreater AST AST                -- Greater than: x > y
    | ALessEq AST AST                 -- Less or equal: x <= y
    | AGreaterEq AST AST              -- Greater or equal: x >= y

    -- Control Flow
    | AIf AST AST AST                 -- If-else: if (cond) then else

    -- Functions
    | AFuncDef String [String] AST    -- Function: fn name(params) { body }
    | ACall String [AST]              -- Call: name(args)
    | AReturn AST                     -- Return: return expr

    -- Statements
    | ASeq [AST]                      -- Sequence: stmt1; stmt2; ...

    deriving (Show, Eq)
```

## Node Types

### Literals

#### Integer Literal

```haskell
AInt :: Int -> AST
```

Represents an integer literal.

Example:
```glados
42          → AInt 42
-15         → AInt (-15)
```

#### Boolean Literal

```haskell
ABool :: Bool -> AST
```

Represents a boolean literal (created by comparisons).

Example (internal representation):
```glados
True        → ABool True
False       → ABool False
```

### Variables

#### Variable Reference

```haskell
AVar :: String -> AST
```

Represents reading a variable's value.

Example:
```glados
x           → AVar "x"
result      → AVar "result"
```

#### Variable Assignment

```haskell
AAssign :: String -> AST -> AST
```

Represents assigning a value to a variable.

Example:
```glados
x = 42              → AAssign "x" (AInt 42)
result = x + y      → AAssign "result" (AAdd (AVar "x") (AVar "y"))
```

### Arithmetic Operations

#### Addition

```haskell
AAdd :: AST -> AST -> AST
```

Example:
```glados
10 + 20     → AAdd (AInt 10) (AInt 20)
x + y       → AAdd (AVar "x") (AVar "y")
```

#### Subtraction

```haskell
ASub :: AST -> AST -> AST
```

Example:
```glados
10 - 5      → ASub (AInt 10) (AInt 5)
```

#### Multiplication

```haskell
AMul :: AST -> AST -> AST
```

Example:
```glados
3 * 4       → AMul (AInt 3) (AInt 4)
```

#### Division

```haskell
ADiv :: AST -> AST -> AST
```

Example:
```glados
10 / 3      → ADiv (AInt 10) (AInt 3)
```

### Comparison Operations

All comparison operations have the same structure:

```haskell
AOp :: AST -> AST -> AST  -- where Op is Eq, NotEq, Less, etc.
```

Examples:
```glados
x == y      → AEq (AVar "x") (AVar "y")
x != y      → ANotEq (AVar "x") (AVar "y")
x < y       → ALess (AVar "x") (AVar "y")
x > y       → AGreater (AVar "x") (AVar "y")
x <= y      → ALessEq (AVar "x") (AVar "y")
x >= y      → AGreaterEq (AVar "x") (AVar "y")
```

### Control Flow

#### If-Else

```haskell
AIf :: AST -> AST -> AST -> AST
     -- condition, then-branch, else-branch
```

Example:
```glados
if (x > 0) {
    return 1
} else {
    return 0
}

→ AIf
    (AGreater (AVar "x") (AInt 0))
    (AReturn (AInt 1))
    (AReturn (AInt 0))
```

### Functions

#### Function Definition

```haskell
AFuncDef :: String -> [String] -> AST -> AST
          -- name, parameters, body
```

Example:
```glados
fn add(a, b) {
    return a + b
}

→ AFuncDef "add" ["a", "b"]
    (AReturn (AAdd (AVar "a") (AVar "b")))
```

#### Function Call

```haskell
ACall :: String -> [AST] -> AST
       -- function name, arguments
```

Example:
```glados
add(10, 20)     → ACall "add" [AInt 10, AInt 20]
factorial(n-1)  → ACall "factorial" [ASub (AVar "n") (AInt 1)]
```

#### Return Statement

```haskell
AReturn :: AST -> AST
```

Example:
```glados
return 42       → AReturn (AInt 42)
return x + y    → AReturn (AAdd (AVar "x") (AVar "y"))
```

### Statement Sequences

```haskell
ASeq :: [AST] -> AST
```

Represents a sequence of statements.

Example:
```glados
{
    x = 10
    y = 20
    return x + y
}

→ ASeq
    [ AAssign "x" (AInt 10)
    , AAssign "y" (AInt 20)
    , AReturn (AAdd (AVar "x") (AVar "y"))
    ]
```

## Complete Example

Source code:
```glados
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fn main() {
    result = factorial(5)
    return result
}
```

AST (formatted for readability):
```haskell
[ AFuncDef "factorial" ["n"]
    (AIf
      (ALess (AVar "n") (AInt 2))
      (AReturn (AInt 1))
      (AReturn
        (AMul
          (AVar "n")
          (ACall "factorial" [ASub (AVar "n") (AInt 1)]))))

, AFuncDef "main" []
    (ASeq
      [ AAssign "result" (ACall "factorial" [AInt 5])
      , AReturn (AVar "result")
      ])
]
```

## AST Properties

### Immutable

ASTs are immutable data structures. Once created, they cannot be modified.

### Recursive

ASTs are recursive structures - nodes can contain other nodes:

```haskell
AAdd (AInt 1) (AMul (AInt 2) (AInt 3))
```

### Type-Safe

The Haskell type system ensures ASTs are well-formed at compile time.

## AST Traversal

### Pattern Matching

ASTs are traversed using pattern matching:

```haskell
evaluate :: AST -> Int
evaluate (AInt n) = n
evaluate (AAdd left right) = evaluate left + evaluate right
evaluate (AMul left right) = evaluate left * evaluate right
-- ... and so on
```

### Recursive Processing

Most AST operations are recursive:

```haskell
compileAST :: AST -> [Instruction]
compileAST (AInt n) = [Push (VInt n)]
compileAST (AAdd l r) = compileAST l ++ compileAST r ++ [BinOp Add]
```

## AST Transformations

### Optimization (Future)

ASTs can be transformed for optimization:

```haskell
optimize :: AST -> AST
optimize (AAdd (AInt a) (AInt b)) = AInt (a + b)  -- Constant folding
optimize (AMul x (AInt 1)) = x                     -- Identity removal
optimize other = other
```

### Pretty Printing

ASTs can be converted back to readable code:

```haskell
pretty :: AST -> String
pretty (AInt n) = show n
pretty (AAdd l r) = "(" ++ pretty l ++ " + " ++ pretty r ++ ")"
```

## AST Validation

### Well-Formedness Checks

- All variables are declared before use (compile-time check)
- Functions are defined before called (compile-time check)
- Types are compatible (runtime check in VM)

## Next Steps

- Learn about [parsing](./parser.md) to create ASTs
- Understand [code generation](./codegen.md) from ASTs
- See the [compiler](./compiler.md) that processes ASTs
