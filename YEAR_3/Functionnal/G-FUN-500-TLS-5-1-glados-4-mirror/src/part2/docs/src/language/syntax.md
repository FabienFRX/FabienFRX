# Syntax

This page describes the complete syntax of the GLaDOS programming language.

## Grammar Notation

The syntax is described using EBNF-like notation:
- `|` means "or" (alternatives)
- `*` means "zero or more"
- `+` means "one or more"
- `?` means "optional"
- `[]` groups elements

## Program Structure

```ebnf
Program ::= FunctionDef+

FunctionDef ::= "fn" Identifier "(" Parameters? ")" "{" Statement* "}"

Parameters ::= Identifier ("," Identifier)*
```

A program consists of one or more function definitions. At least one must be named `main` with no parameters.

## Statements

```ebnf
Statement ::= ReturnStmt
            | Assignment
            | Expression

ReturnStmt ::= "return" Expression

Assignment ::= Identifier "=" Expression
```

### Return Statement

Returns a value from a function:

```glados
return 42
return x + y
return factorial(n - 1)
```

### Assignment

Assigns a value to a variable:

```glados
x = 10
result = factorial(5)
sum = a + b
```

Variables are created on first assignment and are scoped to the current function.

## Expressions

```ebnf
Expression ::= Literal
             | Identifier
             | BinaryOp
             | FunctionCall
             | IfExpression
             | "(" Expression ")"

Literal ::= Integer | Boolean

Integer ::= ["-"] Digit+

Boolean ::= (created by comparison operations)
```

## Operators

### Arithmetic Operators

```ebnf
BinaryOp ::= Expression "+" Expression   // Addition
           | Expression "-" Expression   // Subtraction
           | Expression "*" Expression   // Multiplication
           | Expression "/" Expression   // Division
```

Precedence (highest to lowest):
1. `*`, `/` (multiplication, division)
2. `+`, `-` (addition, subtraction)

Examples:
```glados
x = 2 + 3 * 4      // 14 (not 20)
y = (2 + 3) * 4    // 20
z = 10 / 3         // 3 (integer division)
```

### Comparison Operators

```ebnf
BinaryOp ::= Expression "==" Expression  // Equal
           | Expression "!=" Expression  // Not equal
           | Expression "<" Expression   // Less than
           | Expression ">" Expression   // Greater than
           | Expression "<=" Expression  // Less or equal
           | Expression ">=" Expression  // Greater or equal
```

Comparison operators return boolean values.

Examples:
```glados
x == y     // Equality check
x != y     // Inequality check
x < y      // Less than
x <= y     // Less or equal
x > y      // Greater than
x >= y     // Greater or equal
```

## Control Flow

### If-Else Expression

```ebnf
IfExpression ::= "if" "(" Expression ")" "{" Statement* "}"
                 "else" "{" Statement* "}"
```

Both branches must be present. The condition can be either a boolean or an integer (0 = false, non-zero = true).

Examples:
```glados
if (x > 0) {
    return 1
} else {
    return -1
}

if (x) {           // Integer as condition
    return x
} else {
    return 0
}

// Nested if-else
if (x < 0) {
    return -1
} else {
    if (x > 0) {
        return 1
    } else {
        return 0
    }
}
```

## Functions

### Function Definition

```ebnf
FunctionDef ::= "fn" Identifier "(" Parameters? ")" "{" Statement* "}"

Parameters ::= Identifier ("," Identifier)*
```

Function definitions must:
- Have a unique name
- Have zero or more parameters
- Contain at least one return statement

Examples:
```glados
fn add(a, b) {
    return a + b
}

fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fn main() {
    return 42
}
```

### Function Calls

```ebnf
FunctionCall ::= Identifier "(" Arguments? ")"

Arguments ::= Expression ("," Expression)*
```

Examples:
```glados
add(10, 20)
factorial(5)
max(x, y)
```

The number of arguments must match the number of parameters.

## Identifiers

```ebnf
Identifier ::= Letter (Letter | Digit | "_")*

Letter ::= "a".."z" | "A".."Z" | "_"
Digit ::= "0".."9"
```

Rules for identifiers:
- Must start with a letter or underscore
- Can contain letters, digits, and underscores
- Case-sensitive
- Cannot be a keyword

Valid identifiers:
```glados
x
myVariable
my_var
_private
factorial2
calculateSum
```

Invalid identifiers:
```glados
2fast         // Starts with digit
my-var        // Contains hyphen
fn            // Keyword
if            // Keyword
```

## Keywords

Reserved words that cannot be used as identifiers:

- `fn` - Function definition
- `if` - Conditional expression
- `else` - Alternative branch
- `return` - Return statement

## Literals

### Integer Literals

```ebnf
Integer ::= ["-"] Digit+
```

Examples:
```glados
0
42
-15
1000
```

Note: Negative numbers are technically parsed as the unary minus operator applied to a positive number.

### Boolean Literals

Booleans are not written directly but are the result of comparison operations:

```glados
x < y          // Boolean value (true or false)
a == b         // Boolean value
```

## Whitespace

Whitespace (spaces, tabs, newlines) is ignored except:
- To separate tokens
- Inside string literals (note: GLaDOS doesn't have strings)

These are equivalent:
```glados
fn main(){return 42}

fn main() {
    return 42
}

fn main()
{
    return 42
}
```

## Comments

The current parser does not support comments. Comments in documentation examples are for illustration only.

If you need to "comment out" code, remove it from the source file.

## Operator Precedence

From highest to lowest precedence:

1. **Function calls**: `f(x)`
2. **Unary minus**: `-x`
3. **Multiplicative**: `*`, `/`
4. **Additive**: `+`, `-`
5. **Comparison**: `<`, `>`, `<=`, `>=`
6. **Equality**: `==`, `!=`

Examples:
```glados
x = 2 + 3 * 4          // 14 (multiplication first)
x = (2 + 3) * 4        // 20 (parentheses override)
x = a < b == c < d     // (a < b) == (c < d)
x = -5 * 2             // -10 (unary minus has high precedence)
```

## Operator Associativity

All binary operators are left-associative:

```glados
a - b - c   // (a - b) - c
a / b / c   // (a / b) / c
```

## Complete Example

Here's a complete program demonstrating all syntax elements:

```glados
fn abs(n) {
    if (n < 0) {
        return 0 - n
    } else {
        return n
    }
}

fn max(a, b) {
    if (a > b) {
        return a
    } else {
        return b
    }
}

fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fn main() {
    x = -5
    y = abs(x)
    z = max(y, 10)
    result = factorial(z)
    return result
}
```

## Syntax Errors

Common syntax errors and how to fix them:

### Missing Return

```glados
// ❌ ERROR
fn broken() {
    x = 5
}

// ✓ FIXED
fn fixed() {
    x = 5
    return x
}
```

### Missing Else

```glados
// ❌ ERROR
fn broken(x) {
    if (x > 0) {
        return 1
    }
}

// ✓ FIXED
fn fixed(x) {
    if (x > 0) {
        return 1
    } else {
        return 0
    }
}
```

### Top-Level Code

```glados
// ❌ ERROR
x = 42

fn main() {
    return x
}

// ✓ FIXED
fn main() {
    x = 42
    return x
}
```

### Invalid Identifier

```glados
// ❌ ERROR
2fast = 10

// ✓ FIXED
fast2 = 10
```

## Next Steps

- Learn about [data types](./data-types.md)
- Explore [operators](./operators.md) in detail
- Understand [control flow](./control-flow.md)
- Master [functions](./functions.md)
