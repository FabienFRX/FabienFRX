# Your First Program

Let's write your first GLaDOS program and understand how it works!

## The Simplest Program

Every GLaDOS program must have a `main()` function:

```glados
fn main() {
    return 42
}
```

### Breaking It Down

- `fn main()` - Declares a function named `main` with no parameters
- `return 42` - Returns the integer value 42
- The `main()` function is the entry point - it's automatically called when the program runs

### Compiling and Running

```bash
# Save the code to hello.gld
echo 'fn main() { return 42 }' > hello.gld

# Compile it
./glados-compiler -o hello.bc hello.gld

# Run it
./glados-vm hello.bc
```

Output:
```
42
```

## Adding Variables

Let's add some variables:

```glados
fn main() {
    x = 10
    y = 32
    result = x + y
    return result
}
```

### How It Works

1. `x = 10` - Creates a variable `x` with value 10
2. `y = 32` - Creates a variable `y` with value 32
3. `result = x + y` - Adds x and y, stores in `result`
4. `return result` - Returns the value of `result` (42)

## Using Functions

Functions make code reusable:

```glados
fn add(a, b) {
    return a + b
}

fn main() {
    result = add(10, 32)
    return result
}
```

### Function Definition

- `fn add(a, b)` - Function named `add` with parameters `a` and `b`
- Parameters are automatically bound when the function is called
- `return a + b` - Returns the sum of the parameters

### Function Calls

- `add(10, 32)` - Calls the `add` function with arguments 10 and 32
- The return value is assigned to `result`

## Conditionals

Make decisions with if-else:

```glados
fn max(a, b) {
    if (a > b) {
        return a
    } else {
        return b
    }
}

fn main() {
    result = max(10, 20)
    return result
}
```

### How It Works

1. `if (a > b)` - Checks if `a` is greater than `b`
2. If true, executes the first block (returns `a`)
3. If false, executes the else block (returns `b`)
4. The comparison `a > b` returns a boolean value

## Recursion

Functions can call themselves:

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

### How Recursion Works

1. `factorial(5)` is called
2. Since 5 >= 2, it returns `5 * factorial(4)`
3. `factorial(4)` returns `4 * factorial(3)`
4. This continues until `factorial(1)` returns 1
5. The results multiply back up: 5 × 4 × 3 × 2 × 1 = 120

## Important Rules

### Entry Point

Every program MUST have a `main()` function:
- It must be named exactly `main`
- It must have NO parameters
- It is automatically called when the program starts

### No Top-Level Code

All code must be inside functions:

```glados
// ❌ WRONG - This will fail
x = 10  // Top-level code not allowed!

fn main() {
    return x
}
```

```glados
// ✓ CORRECT
fn main() {
    x = 10  // Code inside a function
    return x
}
```

### Return Statements

All functions must have a return statement:

```glados
fn add(a, b) {
    return a + b  // ✓ Good
}

fn broken() {
    x = 5
    // ❌ Missing return!
}
```

## Type Safety

GLaDOS has two types: integers and booleans.

### Integers

```glados
fn main() {
    x = 42        // Integer
    y = -10       // Negative integer
    z = x + y     // Arithmetic with integers
    return z
}
```

### Booleans

Comparison operators return boolean values:

```glados
fn main() {
    x = 10
    y = 20

    is_equal = x == y       // false
    is_less = x < y         // true

    if (is_less) {
        return 1
    } else {
        return 0
    }
}
```

### Type Rules

- You cannot add/multiply booleans directly
- Comparisons (`==`, `<`, `>`, etc.) return booleans
- `if` statements accept both booleans and integers (0 = false, non-zero = true)

```glados
fn main() {
    a = 5
    b = 10

    // ✓ This works - comparison returns boolean
    if (a < b) {
        return 1
    }

    // ✓ This also works - integer as condition
    if (a) {
        return 2
    }

    // ❌ This fails - cannot multiply booleans
    // result = (a < b) * (b > a)

    return 0
}
```

## Common Patterns

### Boolean to Integer Conversion

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
    comparison = x < y
    result = bool_to_int(comparison)
    return result  // Returns 1
}
```

### Multiple Conditions

```glados
fn check_range(n, min, max) {
    if (n < min) {
        return 0
    } else {
        if (n > max) {
            return 0
        } else {
            return 1
        }
    }
}

fn main() {
    return check_range(15, 10, 20)  // Returns 1 (true)
}
```

## Next Steps

Now that you understand the basics:

- Learn about all [data types](../language/data-types.md)
- Explore [operators](../language/operators.md) in detail
- Understand [control flow](../language/control-flow.md)
- Master [functions](../language/functions.md)
- Check out [examples](../examples/basic.md)
