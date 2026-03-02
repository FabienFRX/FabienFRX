# Language Overview

GLaDOS is a simple, statically-typed programming language with a clean syntax inspired by C and JavaScript.

## Design Philosophy

GLaDOS is designed with these principles:

1. **Simplicity**: Easy to learn and understand
2. **Safety**: Type-safe operations prevent common errors
3. **Clarity**: Readable syntax with minimal complexity
4. **Educational**: Perfect for learning language implementation

## Language Features

### Core Features

- **Integer Arithmetic**: Addition, subtraction, multiplication, division
- **Boolean Logic**: Comparison operators with type-safe booleans
- **Variables**: Named storage with automatic type inference
- **Functions**: First-class functions with parameters and return values
- **Recursion**: Functions can call themselves
- **Conditionals**: If-else statements for control flow

### Type System

GLaDOS has a simple type system with two types:

- **Integer** (`Int`): Whole numbers (positive, negative, or zero)
- **Boolean** (`Bool`): True or false values

Types are inferred automatically and checked at compile time.

### Not Included

To keep the language simple, GLaDOS does NOT have:

- Floating-point numbers
- Strings
- Arrays or lists
- Objects or classes
- Pointers or references
- Mutable data structures beyond variables
- Loops (use recursion instead)
- Modules or imports

## Language Syntax

### Program Structure

Every GLaDOS program follows this structure:

```glados
// Function definitions
fn function_name(param1, param2) {
    // Function body
    return value
}

// Entry point (required)
fn main() {
    // Program starts here
    return result
}
```

### Comments

Currently, GLaDOS does not have built-in comment support in the parser. Comments shown in this documentation are for illustration purposes only.

### Identifiers

Valid identifier names:
- Start with a letter (a-z, A-Z)
- Can contain letters, digits (0-9), and underscores (_)
- Case-sensitive

Examples:
```glados
x           // ✓ Valid
myVar       // ✓ Valid
my_var_2    // ✓ Valid
_private    // ✓ Valid
add2Numbers // ✓ Valid

2fast       // ❌ Invalid (starts with digit)
my-var      // ❌ Invalid (contains hyphen)
```

### Keywords

Reserved words in GLaDOS:
- `fn` - Function definition
- `if` - Conditional statement
- `else` - Alternative branch
- `return` - Return statement

### Literals

Integer literals:
```glados
42          // Positive integer
0           // Zero
-15         // Negative integer (note: parsed as unary minus applied to 15)
```

Boolean literals are created by comparisons:
```glados
x == y      // Boolean: equality check
x < y       // Boolean: less than
```

## Code Examples

### Hello World (Returns 42)

```glados
fn main() {
    return 42
}
```

### Variables and Arithmetic

```glados
fn main() {
    x = 10
    y = 32
    sum = x + y
    product = x * y
    return sum
}
```

### Functions

```glados
fn multiply(a, b) {
    return a * b
}

fn main() {
    result = multiply(6, 7)
    return result
}
```

### Conditionals

```glados
fn abs(n) {
    if (n < 0) {
        return 0 - n
    } else {
        return n
    }
}

fn main() {
    return abs(-25)
}
```

### Recursion

```glados
fn fibonacci(n) {
    if (n < 2) {
        return n
    } else {
        a = n - 1
        b = n - 2
        return fibonacci(a) + fibonacci(b)
    }
}

fn main() {
    return fibonacci(10)
}
```

## Type Safety

GLaDOS enforces type safety at runtime:

### Valid Operations

```glados
fn main() {
    // ✓ Integer arithmetic
    x = 10 + 20
    y = x * 2

    // ✓ Comparisons
    is_greater = x > y

    // ✓ Boolean in conditional
    if (is_greater) {
        return 1
    }

    return 0
}
```

### Type Errors

```glados
fn main() {
    a = 10
    b = 20

    // ❌ Cannot multiply booleans
    // result = (a < b) * (a > b)

    // ✓ Must convert to int first
    x = bool_to_int(a < b)
    y = bool_to_int(a > b)
    result = x * y

    return result
}

fn bool_to_int(b) {
    if (b) { return 1 } else { return 0 }
}
```

## Execution Model

GLaDOS uses a stack-based execution model:

1. **Compilation**: Source code is parsed into an AST, then compiled to bytecode
2. **Loading**: The VM loads the bytecode and function definitions
3. **Execution**: The VM executes bytecode instructions using a stack
4. **Result**: The program returns the value from `main()`

### Entry Point

The `main()` function is required and serves as the entry point:
- Must be defined with zero parameters
- Automatically called when the program starts
- Its return value is the program's result

### Top-Level Code Restriction

All code must be inside functions. Top-level statements are not allowed:

```glados
// ❌ WRONG
x = 42

fn main() {
    return x
}
```

```glados
// ✓ CORRECT
fn main() {
    x = 42
    return x
}
```

## Next Steps

Learn more about specific language features:

- [Syntax](./syntax.md) - Detailed syntax rules
- [Data Types](./data-types.md) - Type system details
- [Operators](./operators.md) - All available operators
- [Control Flow](./control-flow.md) - Conditionals and flow control
- [Functions](./functions.md) - Function definition and calls
