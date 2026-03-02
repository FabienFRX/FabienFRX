# Operators

GLaDOS supports arithmetic and comparison operators.

## Arithmetic Operators

Arithmetic operators work on integers and return integers.

### Addition (`+`)

Adds two integers.

```glados
fn main() {
    x = 10 + 5        // 15
    y = -3 + 7        // 4
    z = x + y         // 19
    return z
}
```

### Subtraction (`-`)

Subtracts the right operand from the left.

```glados
fn main() {
    x = 10 - 5        // 5
    y = 5 - 10        // -5
    z = x - y         // 10
    return z
}
```

### Multiplication (`*`)

Multiplies two integers.

```glados
fn main() {
    x = 10 * 5        // 50
    y = -3 * 4        // -12
    z = x * 2         // 100
    return z
}
```

### Division (`/`)

Divides the left operand by the right (integer division).

```glados
fn main() {
    x = 10 / 3        // 3 (truncates)
    y = 20 / 4        // 5
    z = -10 / 3       // -3 (truncates towards zero)
    return x
}
```

**Important**: Division by zero causes a runtime error.

```glados
fn main() {
    x = 10 / 0        // ❌ Runtime error!
    return x
}
```

## Comparison Operators

Comparison operators work on integers and return booleans.

### Equal To (`==`)

Tests if two values are equal.

```glados
fn main() {
    x = 10
    y = 10
    z = 5

    a = x == y        // true
    b = x == z        // false

    if (a) {
        return 1
    } else {
        return 0
    }
}
```

### Not Equal To (`!=`)

Tests if two values are not equal.

```glados
fn main() {
    x = 10
    y = 5

    result = x != y   // true

    if (result) {
        return 1
    } else {
        return 0
    }
}
```

### Less Than (`<`)

Tests if the left value is less than the right.

```glados
fn main() {
    x = 5
    y = 10

    result = x < y    // true

    if (result) {
        return 1
    } else {
        return 0
    }
}
```

### Greater Than (`>`)

Tests if the left value is greater than the right.

```glados
fn main() {
    x = 10
    y = 5

    result = x > y    // true

    if (result) {
        return 1
    } else {
        return 0
    }
}
```

### Less Than or Equal To (`<=`)

Tests if the left value is less than or equal to the right.

```glados
fn main() {
    x = 10
    y = 10
    z = 5

    a = x <= y        // true (equal)
    b = z <= x        // true (less)
    c = x <= z        // false

    if (a) {
        return 1
    } else {
        return 0
    }
}
```

### Greater Than or Equal To (`>=`)

Tests if the left value is greater than or equal to the right.

```glados
fn main() {
    x = 10
    y = 10
    z = 15

    a = x >= y        // true (equal)
    b = z >= x        // true (greater)
    c = x >= z        // false

    if (a) {
        return 1
    } else {
        return 0
    }
}
```

## Operator Precedence

Operators are evaluated in this order (highest to lowest):

1. **Parentheses**: `()`
2. **Unary minus**: `-x`
3. **Multiplicative**: `*`, `/`
4. **Additive**: `+`, `-`
5. **Comparison**: `<`, `>`, `<=`, `>=`
6. **Equality**: `==`, `!=`

### Examples

```glados
fn main() {
    // Multiplication before addition
    x = 2 + 3 * 4         // 14 (not 20)

    // Parentheses override precedence
    y = (2 + 3) * 4       // 20

    // Division before subtraction
    z = 10 - 8 / 2        // 6 (not 1)

    // Comparison before equality
    a = 5 < 10 == 3 < 7   // true == true → true

    return x
}
```

## Operator Associativity

All binary operators are left-associative (evaluated left-to-right).

```glados
fn main() {
    x = 10 - 5 - 2    // (10 - 5) - 2 = 3
    y = 20 / 4 / 2    // (20 / 4) / 2 = 2
    z = 1 + 2 + 3     // (1 + 2) + 3 = 6

    return x
}
```

## Unary Operators

### Unary Minus (`-`)

Negates a number.

```glados
fn main() {
    x = 10
    y = -x            // -10
    z = -y            // 10

    a = -(5 + 3)      // -8

    return z
}
```

## Type Restrictions

### Arithmetic on Integers Only

```glados
fn main() {
    x = 10
    y = 20

    // ✓ Valid: integers
    sum = x + y

    // ❌ Invalid: cannot add booleans
    // result = (x < y) + (x > y)

    return sum
}
```

### Comparisons on Integers Only

```glados
fn main() {
    x = 10
    y = 20

    // ✓ Valid: comparing integers
    is_less = x < y

    // ❌ Invalid: cannot compare booleans
    // result = (x < y) < (x > y)

    if (is_less) {
        return 1
    } else {
        return 0
    }
}
```

## Complex Expressions

You can combine operators to create complex expressions:

```glados
fn main() {
    x = 10
    y = 20
    z = 5

    // Complex arithmetic
    result = (x + y) * z - (x / z)     // (10+20)*5 - (10/5) = 148

    // Complex comparisons
    is_in_range = x > 0 == y < 100     // (x > 0) == (y < 100)

    // Nested expressions
    complex = ((x + y) * (z - 2)) / (x - z)

    return result
}
```

## Common Patterns

### Absolute Value

```glados
fn abs(n) {
    if (n < 0) {
        return 0 - n      // or: return -n
    } else {
        return n
    }
}

fn main() {
    return abs(-25)       // 25
}
```

### Minimum

```glados
fn min(a, b) {
    if (a < b) {
        return a
    } else {
        return b
    }
}

fn main() {
    return min(10, 20)    // 10
}
```

### Maximum

```glados
fn max(a, b) {
    if (a > b) {
        return a
    } else {
        return b
    }
}

fn main() {
    return max(10, 20)    // 20
}
```

### Sign Function

```glados
fn sign(n) {
    if (n < 0) {
        return -1
    } else {
        if (n > 0) {
            return 1
        } else {
            return 0
        }
    }
}

fn main() {
    return sign(-42)      // -1
}
```

### Clamp

```glados
fn clamp(value, min_val, max_val) {
    if (value < min_val) {
        return min_val
    } else {
        if (value > max_val) {
            return max_val
        } else {
            return value
        }
    }
}

fn main() {
    return clamp(15, 10, 20)    // 15 (within range)
}
```

## Logical Operations (Emulated)

GLaDOS doesn't have built-in logical operators, but you can implement them:

### AND

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
```

### OR

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
```

### NOT

```glados
fn not(b) {
    if (b) {
        return 0
    } else {
        return 1
    }
}
```

## Operator Summary

| Operator | Type | Operands | Result | Description |
|----------|------|----------|--------|-------------|
| `+` | Arithmetic | Int, Int | Int | Addition |
| `-` | Arithmetic | Int, Int | Int | Subtraction |
| `*` | Arithmetic | Int, Int | Int | Multiplication |
| `/` | Arithmetic | Int, Int | Int | Division |
| `-` | Unary | Int | Int | Negation |
| `==` | Comparison | Int, Int | Bool | Equal to |
| `!=` | Comparison | Int, Int | Bool | Not equal to |
| `<` | Comparison | Int, Int | Bool | Less than |
| `>` | Comparison | Int, Int | Bool | Greater than |
| `<=` | Comparison | Int, Int | Bool | Less or equal |
| `>=` | Comparison | Int, Int | Bool | Greater or equal |

## Next Steps

- Understand [control flow](./control-flow.md) using comparison operators
- Learn about [functions](./functions.md) and how to use operators in them
- See [examples](../examples/basic.md) demonstrating operator usage
