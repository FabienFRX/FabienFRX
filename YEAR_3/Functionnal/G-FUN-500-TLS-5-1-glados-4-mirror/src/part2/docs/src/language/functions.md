# Functions

Functions are the building blocks of GLaDOS programs. All code must be inside functions, and every program requires a `main()` function as its entry point.

## Function Definition

### Syntax

```glados
fn function_name(param1, param2, ...) {
    // Function body
    return value
}
```

### Components

- `fn` - Keyword to define a function
- `function_name` - Name of the function (must be a valid identifier)
- `(param1, param2, ...)` - Parameters (zero or more)
- `{ ... }` - Function body
- `return value` - Return statement (required)

### Example

```glados
fn add(a, b) {
    return a + b
}

fn main() {
    result = add(10, 20)
    return result
}
```

## Parameters

### Zero Parameters

```glados
fn get_answer() {
    return 42
}

fn main() {
    answer = get_answer()
    return answer
}
```

### One Parameter

```glados
fn square(n) {
    return n * n
}

fn main() {
    return square(7)    // 49
}
```

### Multiple Parameters

```glados
fn multiply(a, b) {
    return a * b
}

fn power(base, exp) {
    if (exp <= 0) {
        return 1
    } else {
        return base * power(base, exp - 1)
    }
}

fn main() {
    return power(2, 10)    // 1024
}
```

## Function Calls

### Syntax

```glados
function_name(arg1, arg2, ...)
```

### Calling Functions

```glados
fn add(a, b) {
    return a + b
}

fn main() {
    x = add(5, 10)          // Assign result to variable
    y = add(x, 20)          // Use variable as argument
    z = add(add(1, 2), 3)   // Nested calls
    return z
}
```

### Argument Matching

The number of arguments must match the number of parameters:

```glados
fn add(a, b) {
    return a + b
}

fn main() {
    x = add(10, 20)     // ✓ Correct: 2 arguments
    // y = add(10)      // ❌ Error: too few arguments
    // z = add(10, 20, 30)  // ❌ Error: too many arguments
    return x
}
```

## Return Statements

Every function must have a `return` statement.

### Simple Return

```glados
fn get_answer() {
    return 42
}
```

### Returning Expressions

```glados
fn sum(a, b, c) {
    return a + b + c
}

fn main() {
    return sum(10, 20, 30)    // 60
}
```

### Multiple Return Points

```glados
fn abs(n) {
    if (n < 0) {
        return 0 - n
    } else {
        return n
    }
}
```

## The `main()` Function

Every GLaDOS program must have a `main()` function.

### Requirements

- Must be named exactly `main`
- Must have **zero parameters**
- Must have a `return` statement
- Serves as the program entry point

### Example

```glados
fn main() {
    return 42
}
```

### Invalid `main()` Functions

```glados
// ❌ ERROR: main() cannot have parameters
fn main(x) {
    return x
}

// ❌ ERROR: wrong name
fn Main() {
    return 42
}

// ❌ ERROR: missing return
fn main() {
    x = 42
}
```

## Recursion

Functions can call themselves, enabling iterative computations.

### Basic Recursion

```glados
fn countdown(n) {
    if (n <= 0) {
        return 0
    } else {
        return n + countdown(n - 1)
    }
}

fn main() {
    return countdown(5)    // 15 (5+4+3+2+1)
}
```

### Factorial

```glados
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fn main() {
    return factorial(5)    // 120
}
```

### Fibonacci

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
    return fibonacci(10)    // 55
}
```

### Mutual Recursion

Functions can call each other:

```glados
fn is_even(n) {
    if (n == 0) {
        return 1
    } else {
        return is_odd(n - 1)
    }
}

fn is_odd(n) {
    if (n == 0) {
        return 0
    } else {
        return is_even(n - 1)
    }
}

fn main() {
    return is_even(42)    // 1 (true)
}
```

## Variable Scope

Variables are scoped to the function in which they're defined.

### Local Variables

```glados
fn func1() {
    x = 10
    return x
}

fn func2() {
    x = 20       // Different x, local to func2
    return x
}

fn main() {
    a = func1()  // 10
    b = func2()  // 20
    return a + b // 30
}
```

### Parameter Scope

```glados
fn add(a, b) {
    // a and b are local to this function
    sum = a + b
    return sum
}

fn main() {
    result = add(10, 20)
    // a and b are not accessible here
    return result
}
```

## Function Composition

Functions can call other functions:

```glados
fn square(n) {
    return n * n
}

fn sum_of_squares(a, b) {
    return square(a) + square(b)
}

fn main() {
    return sum_of_squares(3, 4)    // 25 (9 + 16)
}
```

## Common Function Patterns

### Helper Functions

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
    result = bool_to_int(x < y)
    return result    // 1
}
```

### Predicate Functions

```glados
fn is_positive(n) {
    if (n > 0) {
        return 1
    } else {
        return 0
    }
}

fn is_negative(n) {
    if (n < 0) {
        return 1
    } else {
        return 0
    }
}

fn main() {
    return is_positive(42)    // 1
}
```

### Utility Functions

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

fn clamp(value, low, high) {
    if (value < low) {
        return low
    } else {
        if (value > high) {
            return high
        } else {
            return value
        }
    }
}

fn main() {
    return clamp(15, 10, 20)    // 15
}
```

## Tail Recursion

Tail-recursive functions call themselves as their last operation:

```glados
fn factorial_helper(n, acc) {
    if (n < 2) {
        return acc
    } else {
        return factorial_helper(n - 1, n * acc)
    }
}

fn factorial(n) {
    return factorial_helper(n, 1)
}

fn main() {
    return factorial(5)    // 120
}
```

## Advanced Examples

### GCD (Greatest Common Divisor)

```glados
fn mod(a, b) {
    return a - (a / b) * b
}

fn gcd(a, b) {
    if (b == 0) {
        return a
    } else {
        return gcd(b, mod(a, b))
    }
}

fn main() {
    return gcd(48, 18)    // 6
}
```

### Ackermann Function

```glados
fn ackermann(m, n) {
    if (m == 0) {
        return n + 1
    } else {
        if (n == 0) {
            return ackermann(m - 1, 1)
        } else {
            inner = ackermann(m, n - 1)
            return ackermann(m - 1, inner)
        }
    }
}

fn main() {
    return ackermann(3, 3)    // 61
}
```

### Power Function

```glados
fn power(base, exp) {
    if (exp <= 0) {
        return 1
    } else {
        if (exp == 1) {
            return base
        } else {
            return base * power(base, exp - 1)
        }
    }
}

fn main() {
    return power(2, 10)    // 1024
}
```

## Best Practices

### Descriptive Names

```glados
// ✓ Good: descriptive names
fn calculate_area(width, height) {
    return width * height
}

// ❌ Bad: unclear names
fn f(a, b) {
    return a * b
}
```

### Single Responsibility

```glados
// ✓ Good: each function does one thing
fn square(n) {
    return n * n
}

fn sum_of_squares(a, b) {
    return square(a) + square(b)
}

// ❌ Bad: doing too much in one function
fn everything(a, b, c, d) {
    x = a * a
    y = b * b
    z = c * c
    w = d * d
    return x + y + z + w
}
```

### Base Cases First

```glados
// ✓ Good: base case first
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}
```

## Common Mistakes

### Missing Return

```glados
// ❌ ERROR
fn broken(x) {
    x + 10
}

// ✓ CORRECT
fn fixed(x) {
    return x + 10
}
```

### Wrong Parameter Count

```glados
fn add(a, b) {
    return a + b
}

fn main() {
    // ❌ ERROR: wrong number of arguments
    // result = add(10)

    // ✓ CORRECT
    result = add(10, 20)
    return result
}
```

### Infinite Recursion

```glados
// ❌ ERROR: infinite recursion (no base case)
fn broken(n) {
    return broken(n - 1)
}

// ✓ CORRECT: has base case
fn fixed(n) {
    if (n <= 0) {
        return 0
    } else {
        return fixed(n - 1)
    }
}
```

## Summary

- **All code must be in functions**
- **`main()` is required** with zero parameters
- **Parameters** can be zero or more
- **Return required** in all functions
- **Recursion** is the primary iteration mechanism
- **Scope** is function-local
- **Composition** enables building complex behavior

## Next Steps

- See [examples](../examples/functions.md) of advanced function usage
- Learn about [recursion patterns](../examples/advanced.md)
- Explore the [compiler](../architecture/compiler.md) to understand how functions are compiled
