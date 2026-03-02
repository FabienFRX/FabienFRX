# Control Flow

GLaDOS provides conditional execution through if-else statements.

## If-Else Statements

The if-else statement is the only control flow construct in GLaDOS. It allows you to execute different code based on a condition.

### Syntax

```glados
if (condition) {
    // Code to execute if condition is true
} else {
    // Code to execute if condition is false
}
```

Both the `if` branch and the `else` branch are **required**.

### Basic Example

```glados
fn main() {
    x = 10

    if (x > 5) {
        return 1
    } else {
        return 0
    }
}
```

## Conditions

The condition in an if-else statement can be:

### Boolean Condition

Use comparison operators to create boolean conditions:

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

### Integer Condition

You can also use integers as conditions:
- `0` is treated as false
- Any non-zero value is treated as true

```glados
fn main() {
    x = 42

    if (x) {           // Non-zero, treated as true
        return 1
    } else {
        return 0
    }
}
```

## Nested If-Else

You can nest if-else statements to handle multiple conditions:

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
    return sign(42)    // Returns 1
}
```

## Common Patterns

### Range Checking

```glados
fn in_range(n, min_val, max_val) {
    if (n < min_val) {
        return 0
    } else {
        if (n > max_val) {
            return 0
        } else {
            return 1
        }
    }
}

fn main() {
    return in_range(15, 10, 20)    // Returns 1 (true)
}
```

### Multiple Conditions

```glados
fn grade(score) {
    if (score >= 90) {
        return 5        // A
    } else {
        if (score >= 80) {
            return 4    // B
        } else {
            if (score >= 70) {
                return 3    // C
            } else {
                if (score >= 60) {
                    return 2    // D
                } else {
                    return 1    // F
                }
            }
        }
    }
}

fn main() {
    return grade(85)    // Returns 4 (B)
}
```

### Boolean Logic

Since GLaDOS doesn't have `&&` and `||` operators, implement them as functions:

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

fn both_positive(a, b) {
    return and(a > 0, b > 0)
}

fn main() {
    return both_positive(5, 10)    // Returns 1
}
```

## Comparison Operators in Conditions

All comparison operators return boolean values suitable for use in conditions:

### Equality

```glados
fn is_equal(a, b) {
    if (a == b) {
        return 1
    } else {
        return 0
    }
}
```

### Inequality

```glados
fn is_different(a, b) {
    if (a != b) {
        return 1
    } else {
        return 0
    }
}
```

### Ordering

```glados
fn max(a, b) {
    if (a > b) {
        return a
    } else {
        return b
    }
}

fn min(a, b) {
    if (a < b) {
        return a
    } else {
        return b
    }
}
```

## If-Else as Expressions

In GLaDOS, if-else can be used as part of an expression by storing its result:

```glados
fn abs(n) {
    result = 0
    if (n < 0) {
        result = 0 - n
    } else {
        result = n
    }
    return result
}

fn main() {
    return abs(-25)    // Returns 25
}
```

However, it's more idiomatic to return directly:

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

## Looping with Recursion

GLaDOS doesn't have traditional loop constructs. Use recursion instead:

### Counting Down

```glados
fn countdown(n) {
    if (n <= 0) {
        return 0
    } else {
        return n + countdown(n - 1)
    }
}

fn main() {
    return countdown(10)    // Returns 55 (sum of 1 to 10)
}
```

### Iteration Pattern

```glados
fn sum_to_n(n) {
    if (n <= 0) {
        return 0
    } else {
        return n + sum_to_n(n - 1)
    }
}

fn main() {
    return sum_to_n(100)    // Returns 5050
}
```

## Early Return Pattern

Use early returns to avoid deep nesting:

```glados
fn validate(n) {
    if (n < 0) {
        return 0    // Invalid
    } else {
        if (n > 100) {
            return 0    // Invalid
        } else {
            return 1    // Valid
        }
    }
}
```

Can be simplified conceptually (though GLaDOS requires explicit else blocks):

```glados
fn validate(n) {
    if (n < 0) {
        return 0
    } else {
        if (n > 100) {
            return 0
        } else {
            return 1
        }
    }
}
```

## Common Mistakes

### Missing Else Branch

```glados
// ❌ ERROR: Missing else branch
fn broken(x) {
    if (x > 0) {
        return 1
    }
}

// ✓ CORRECT: Both branches present
fn fixed(x) {
    if (x > 0) {
        return 1
    } else {
        return 0
    }
}
```

### Type Errors in Conditions

```glados
fn main() {
    x = 10
    y = 20

    // ❌ ERROR: Cannot use arithmetic result of booleans
    // if ((x < y) + (x > y)) {
    //     return 1
    // } else {
    //     return 0
    // }

    // ✓ CORRECT: Use boolean directly
    if (x < y) {
        return 1
    } else {
        return 0
    }
}
```

## Advanced Patterns

### Guarded Recursion

```glados
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fn main() {
    return factorial(5)    // Returns 120
}
```

### Multiple Base Cases

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
    return fibonacci(10)    // Returns 55
}
```

### Complex Conditions

```glados
fn between(n, low, high) {
    if (n >= low) {
        if (n <= high) {
            return 1
        } else {
            return 0
        }
    } else {
        return 0
    }
}

fn main() {
    return between(15, 10, 20)    // Returns 1
}
```

## Performance Considerations

### Tail Recursion

While GLaDOS doesn't guarantee tail call optimization, you can write tail-recursive functions:

```glados
fn sum_helper(n, acc) {
    if (n <= 0) {
        return acc
    } else {
        return sum_helper(n - 1, acc + n)
    }
}

fn sum_to_n(n) {
    return sum_helper(n, 0)
}

fn main() {
    return sum_to_n(100)
}
```

### Avoiding Deep Recursion

For very large inputs, deep recursion may cause stack overflow. Keep recursion depth reasonable.

## Summary

- **Only control structure**: if-else statements
- **Both branches required**: Must have both if and else
- **Conditions**: Can be boolean or integer (0 = false)
- **Nesting**: Can nest if-else statements
- **Recursion**: Use recursion instead of loops
- **Comparisons**: Use comparison operators to create conditions

## Next Steps

- Master [functions](./functions.md) and recursion
- See [examples](../examples/basic.md) of control flow patterns
- Learn about [recursion examples](../examples/functions.md)
