# GLaDOS LISP Interpreter Documentation

## Table of Contents
1. [Overview](#overview)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Language Syntax](#language-syntax)
5. [Core Concepts](#core-concepts)
6. [Built-in Functions](#built-in-functions)
7. [Examples](#examples)
8. [Project Architecture](#project-architecture)
9. [Testing](#testing)
10. [Error Handling](#error-handling)
11. [Development](#development)

---

## Overview

GLaDOS is a minimalist LISP interpreter implemented in Haskell. It provides a functional programming environment with support for:
- First-class functions (lambdas)
- Lexical scoping
- Recursive function definitions
- Conditional expressions
- Basic arithmetic and comparison operators

### Project Information
- **Language**: Haskell
- **Build System**: Stack
- **Version**: 0.1.0.0
- **Author**: Gobijan, Andi, Fabien, Marceau
- **Institution**: EPITECH

---

### Prerequisites
- GHC (Glasgow Haskell Compiler) 9.6.6 or compatible
- Stack build tool
- Make

---

## Language Syntax

### S-Expressions

GLaDOS uses S-expressions (symbolic expressions) as its fundamental syntax:

#### Atoms
- **Integers**: Signed integers in base 10
  ```scheme
  42
  -17
  0
  ```

- **Booleans**: True and false values
  ```scheme
  #t  ; true
  #f  ; false
  ```

- **Symbols**: Any string that is not a number
  ```scheme
  x
  foo-bar
  +
  eq?
  ```

#### Lists
Lists are enclosed in parentheses and contain zero or more expressions:
```scheme
()                  ; empty list
(1 2 3)            ; list of numbers
(+ 1 2)            ; function application
((foo bar) baz)    ; nested lists
```

### Special Forms

#### define
Binds a value to a symbol:
```scheme
; Variable definition
(define x 42)

; Function definition (syntactic sugar)
(define (square x)
  (* x x))

; Equivalent lambda form
(define square
  (lambda (x)
    (* x x)))
```

#### lambda
Creates anonymous functions:
```scheme
(lambda (x) (* x 2))           ; single parameter
(lambda (x y) (+ x y))         ; multiple parameters
(lambda () 42)                 ; no parameters

; Immediately invoked lambda
((lambda (x) (* x x)) 5)       ; returns 25
```

#### if
Conditional expression:
```scheme
(if condition then-expr else-expr)

; Examples
(if #t 1 2)                    ; returns 1
(if (< x 10) "small" "large")
```

---

## Core Concepts

### Types

GLaDOS supports the following types:

| Type | Description | Example |
|------|-------------|---------|
| Integer | 64-bit signed integers | `42`, `-17` |
| Boolean | Truth values | `#t`, `#f` |
| Symbol | Variable names | `x`, `foo` |
| Procedure | Functions/lambdas | `#<procedure>` |

### Evaluation Rules

1. **Integers** and **Booleans** evaluate to themselves
2. **Symbols** evaluate to their bound values (error if unbound)
3. **Lists** are evaluated as function calls:
   - First element is the function
   - Remaining elements are arguments
   - Arguments are evaluated before application
4. **Special forms** (`define`, `lambda`, `if`) have unique evaluation rules

### Scope and Binding

- **Lexical scoping**: Variables are resolved in their definition environment
- **Closures**: Functions capture their definition environment
- **Self-referential definitions**: Recursive functions are supported

Example:
```scheme
(define (make-adder n)
  (lambda (x) (+ x n)))

(define add5 (make-adder 5))
(add5 3)  ; returns 8
```

---

## Built-in Functions

### Arithmetic Operators

| Function | Description | Example | Result |
|----------|-------------|---------|--------|
| `+` | Addition | `(+ 3 4)` | `7` |
| `-` | Subtraction | `(- 10 3)` | `7` |
| `*` | Multiplication | `(* 6 7)` | `42` |
| `div` | Integer division | `(div 10 3)` | `3` |
| `mod` | Modulo | `(mod 10 3)` | `1` |

### Comparison Operators

| Function | Description | Example | Result |
|----------|-------------|---------|--------|
| `eq?` | Equality test | `(eq? 5 5)` | `#t` |
| `<` | Less than | `(< 3 5)` | `#t` |

Note: All arithmetic and comparison operators require exactly 2 arguments.

---

## Examples

### Basic Arithmetic
```scheme
(+ 1 2)                        ; 3
(* (+ 2 3) (- 10 6))          ; 20
(div (+ 8 4) 3)               ; 4
```

### Variable Definitions
```scheme
(define x 10)
(define y 20)
(+ x y)                        ; 30
```

### Function Definitions
```scheme
; Simple function
(define (double x)
  (* x 2))
(double 21)                    ; 42

; Recursive function - factorial
(define (fact n)
  (if (eq? n 0)
      1
      (* n (fact (- n 1)))))
(fact 5)                       ; 120

; Higher-order function
(define (apply-twice f x)
  (f (f x)))
(apply-twice double 5)        ; 20
```

### Conditional Logic
```scheme
; Maximum function
(define (max a b)
  (if (< a b) b a))
(max 3 7)                      ; 7

; Absolute value
(define (abs x)
  (if (< x 0)
      (- 0 x)
      x))
(abs -5)                       ; 5
```

### Advanced Examples

#### Fibonacci Sequence
```scheme
(define (fib n)
  (if (< n 2)
      n
      (+ (fib (- n 1))
         (fib (- n 2)))))
(fib 10)                       ; 55
```

#### Power Function
```scheme
(define (pow base exp)
  (if (eq? exp 0)
      1
      (* base (pow base (- exp 1)))))
(pow 2 8)                      ; 256
```

#### List-like Operations (using nested functions)
```scheme
(define (range-sum start end)
  (if (< end start)
      0
      (+ start (range-sum (+ start 1) end)))
(range-sum 1 10)               ; 55
```

### Module Responsibilities

#### Main.hs
- Program entry point
- Command-line argument handling
- REPL implementation
- File/stdin reading
- Error handling and exit codes

#### Parser.hs
- Parses text input into S-expressions
- Uses Parsec library
- Handles:
  - Integers (signed)
  - Booleans (#t, #f)
  - Symbols
  - Lists (nested)
  - Whitespace and comments

#### AST.hs
- Data type definitions:
  - `SExpr`: Raw S-expressions
  - `AST`: Abstract syntax tree
  - `Value`: Runtime values
  - `Env`: Environment (variable bindings)
- S-expression to AST conversion
- Value printing functions

#### Evaluator.hs
- Expression evaluation
- Environment management
- Built-in function implementations
- Special form handling
- Closure creation and application

### Data Flow

```
Input Text → Parser → SExpr → AST Converter → AST → Evaluator → Value → Output
                         ↑                            ↑
                     Parse Error                 Runtime Error
```

---


### Test Coverage
The test suite covers:
- Parser functionality
  - Atom parsing (integers, booleans, symbols)
  - List parsing (empty, simple, nested)
  - Multiple expression parsing
- AST conversion
  - All node types
  - Special forms
- Evaluator
  - Literals
  - Arithmetic operations
  - Comparisons
  - Variable binding
  - Conditionals
  - Lambda expressions
  - Integration tests (factorial, etc.)

### Example Test Cases
```haskell
-- Parser tests
parseProgram "42" == Right [SInt 42]
parseProgram "(+ 1 2)" == Right [SList [SSymbol "+", SInt 1, SInt 2]]

-- Evaluator tests
eval initialEnv (AInt 42) == Right (VInt 42, initialEnv)
eval initialEnv (Call (ASymbol "+") [AInt 1, AInt 2]) == Right (VInt 3, ...)
```

---

## Error Handling

### Error Types

#### Parse Errors
```
*** PARSE ERROR: unexpected ")"
expecting letter or symbolChar
```

#### Runtime Errors
```
*** ERROR: variable foo is not bound.
*** ERROR: division by zero
*** ERROR: if condition must be a boolean
*** ERROR: attempting to call a non-procedure
*** ERROR: procedure expects 2 arguments, got 3
```

### Error Recovery
- Errors immediately halt execution
- Exit code 84 is returned
- Error messages are written to stderr
- In REPL mode, errors don't exit the program

---

## Development

### Code Style
- Pure functional programming (no mutable state)
- Extensive use of Maybe and Either for error handling
- Pattern matching for control flow
- Type signatures for all top-level functions

### Adding New Features

#### Adding a Built-in Function
1. Add the function name to `initialEnv` in Evaluator.hs
2. Implement the function in `applyBuiltin`

Example:
```haskell
-- In initialEnv
(">", VBuiltin ">")

-- In applyBuiltin
applyBuiltin ">" [VInt a, VInt b] env = Right (VBool (a > b), env)
```

#### Adding a Special Form
1. Add new AST constructor in AST.hs
2. Add parsing logic in sexprToAST
3. Add evaluation logic in eval

### Dependencies
- **base**: Core Haskell libraries
- **parsec**: Parser combinators
- **containers**: Data structures
- **hspec**: Testing framework (dev)

### Building for Production
```bash
stack build --optimize
```

---

## Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| "variable is not bound" | Ensure variable is defined before use |
| "division by zero" | Check denominator before division |
| Parse errors | Check parentheses matching |
| Stack overflow | Recursive function may lack base case |

### Performance Considerations
- Recursive functions are not tail-call optimized
- Large recursive computations may cause stack overflow
- Consider iterative alternatives for deep recursion

---

## Future Enhancements (Part 2)

Planned features for Part 2:
- Custom syntax (non S-expression based)
- Virtual machine and bytecode compiler
- Additional data types (lists, strings, floats)
- More built-in functions
- Type safety improvements
- Performance optimizations

---

## Repository
https://github.com/EpitechPGE3-2025/G-FUN-500-TLS-5-1-glados-4
