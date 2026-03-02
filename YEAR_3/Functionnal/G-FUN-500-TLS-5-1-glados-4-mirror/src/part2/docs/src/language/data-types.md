# Data Types

GLaDOS has a simple type system with two primitive types: integers and booleans.

## Integer Type

### Description

Integers represent whole numbers (positive, negative, or zero).

### Syntax

```glados
42          // Positive integer
0           // Zero
-15         // Negative integer
1000        // Large integer
```

### Range

Integers are implemented using Haskell's `Int` type, which typically provides:
- **32-bit systems**: -2,147,483,648 to 2,147,483,647
- **64-bit systems**: -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

### Operations

Integers support arithmetic operations:

```glados
fn main() {
    a = 10
    b = 3

    sum = a + b        // 13
    diff = a - b       // 7
    prod = a * b       // 30
    quot = a / b       // 3 (integer division)

    return sum
}
```

### Integer Division

Division truncates towards zero:

```glados
fn main() {
    x = 10 / 3     // 3
    y = -10 / 3    // -3
    z = 10 / 2     // 5
    return x
}
```

### Negative Numbers

Negative numbers are created using the unary minus operator:

```glados
fn main() {
    x = -42
    y = 0 - 10     // Another way to write -10
    z = -x         // 42 (negation of -42)
    return z
}
```

## Boolean Type

### Description

Booleans represent truth values: true or false.

### Creation

Booleans are created by comparison operations:

```glados
fn main() {
    x = 10
    y = 20

    is_equal = x == y      // false
    is_less = x < y        // true
    is_greater = x > y     // false

    return 1
}
```

### Comparison Operators

All comparison operators return boolean values:

```glados
fn main() {
    a = 10
    b = 20

    eq = a == b      // Equal to
    neq = a != b     // Not equal to
    lt = a < b       // Less than
    gt = a > b       // Greater than
    lte = a <= b     // Less than or equal
    gte = a >= b     // Greater than or equal

    return 1
}
```

### Using Booleans

Booleans are primarily used in conditional expressions:

```glados
fn main() {
    x = 10
    y = 20

    is_less = x < y

    if (is_less) {
        return 1
    } else {
        return 0
    }
}
```

## Type Safety

GLaDOS enforces type safety to prevent invalid operations.

### Valid Operations

```glados
fn main() {
    // ✓ Integer arithmetic
    x = 10 + 20
    y = x * 2

    // ✓ Comparisons (return booleans)
    is_greater = x > y

    // ✓ Booleans in conditionals
    if (is_greater) {
        return 1
    } else {
        return 0
    }
}
```

### Type Errors

```glados
fn main() {
    x = 10
    y = 20

    // ❌ ERROR: Cannot perform arithmetic on booleans
    // result = (x < y) + (x > y)

    // ❌ ERROR: Cannot multiply booleans
    // result = (x < y) * (x > y)

    return 0
}
```

## Type Conversion

### Boolean to Integer

To convert a boolean to an integer, use a helper function:

```glados
fn bool_to_int(b) {
    if (b) {
        return 1
    } else {
        return 0
    }
}

fn main() {
    x = 10
    y = 20

    is_less = x < y          // Boolean: true
    as_int = bool_to_int(is_less)  // Integer: 1

    return as_int
}
```

### Integer to Boolean (Implicit)

Integers can be used as boolean conditions in `if` statements:
- `0` is treated as false
- Any non-zero value is treated as true

```glados
fn main() {
    x = 42

    if (x) {           // 42 is non-zero, treated as true
        return 1
    } else {
        return 0
    }
}
```

## Type Checking

Type checking happens at runtime in the VM. When you try to perform an invalid operation, you'll get a runtime error.

### Valid Type Combinations

| Operation | Left Type | Right Type | Result Type |
|-----------|-----------|------------|-------------|
| `+`       | Int       | Int        | Int         |
| `-`       | Int       | Int        | Int         |
| `*`       | Int       | Int        | Int         |
| `/`       | Int       | Int        | Int         |
| `==`      | Int       | Int        | Bool        |
| `!=`      | Int       | Int        | Bool        |
| `<`       | Int       | Int        | Bool        |
| `>`       | Int       | Int        | Bool        |
| `<=`      | Int       | Int        | Bool        |
| `>=`      | Int       | Int        | Bool        |
| `if`      | Bool/Int  | -          | -           |

### Error Examples

These will cause runtime type errors:

```glados
fn main() {
    x = 10 < 20        // Boolean: true
    y = 30 < 40        // Boolean: true

    // ❌ Runtime error: Cannot add booleans
    // result = x + y

    return 0
}
```

## Working with Types

### Example: Logical AND

Since GLaDOS doesn't have built-in logical operators, implement them using functions:

```glados
fn and(a, b) {
    if (a) {
        if (b) {
            return 1
        } else {
            return 0
        }
    } else {
        return 0
    }
}

fn main() {
    x = 10
    y = 20

    both_positive = and(x > 0, y > 0)

    return both_positive
}
```

### Example: Logical OR

```glados
fn or(a, b) {
    if (a) {
        return 1
    } else {
        if (b) {
            return 1
        } else {
            return 0
        }
    }
}

fn main() {
    x = -5
    y = 10

    any_positive = or(x > 0, y > 0)

    return any_positive
}
```

### Example: Logical NOT

```glados
fn not(b) {
    if (b) {
        return 0
    } else {
        return 1
    }
}

fn main() {
    x = 10

    is_not_zero = not(x == 0)

    return is_not_zero
}
```

## Type Annotations

GLaDOS does not have explicit type annotations. Types are:
- Inferred from literals and operations
- Checked at runtime by the VM

```glados
fn main() {
    x = 42         // Inferred as Int
    y = x < 50     // Inferred as Bool
    z = x + 10     // Inferred as Int

    return z
}
```

## Future Extensions

Potential future type system enhancements:
- Floating-point numbers
- Strings
- Arrays
- Custom data types (structs)
- Static type checking at compile time

## Summary

- **Two types**: Int and Bool
- **Type-safe**: Invalid operations cause runtime errors
- **Inference**: Types are inferred automatically
- **Conversion**: Use helper functions to convert between types
- **Runtime checking**: Type errors are caught during execution

## Next Steps

- Learn about [operators](./operators.md) for each type
- Understand [control flow](./control-flow.md) with booleans
- See [examples](../examples/basic.md) using different types
