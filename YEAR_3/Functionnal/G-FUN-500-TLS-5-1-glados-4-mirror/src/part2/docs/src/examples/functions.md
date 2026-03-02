# Functions and Recursion

Examples demonstrating functions, recursion, and function composition.

## Simple Function

```glados
fn add(a, b) {
    return a + b
}

fn main() {
    result = add(10, 20)
    return result
}
```

**Output**: `30`

## Function Composition

```glados
fn square(n) {
    return n * n
}

fn sum_of_squares(a, b) {
    return square(a) + square(b)
}

fn main() {
    return sum_of_squares(3, 4)
}
```

**Output**: `25` (9 + 16)

## Factorial (Recursive)

```glados
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

fn main() {
    return factorial(5)
}
```

**Output**: `120` (5! = 5×4×3×2×1)

## Fibonacci (Recursive)

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

**Output**: `55`

## Power Function

```glados
fn power(base, exp) {
    if (exp <= 0) {
        return 1
    } else {
        return base * power(base, exp - 1)
    }
}

fn main() {
    return power(2, 10)
}
```

**Output**: `1024` (2^10)

## Sum to N

```glados
fn sum_to_n(n) {
    if (n <= 0) {
        return 0
    } else {
        return n + sum_to_n(n - 1)
    }
}

fn main() {
    return sum_to_n(100)
}
```

**Output**: `5050` (sum of 1 to 100)

## Tail-Recursive Factorial

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
    return factorial(5)
}
```

**Output**: `120`

## Multiple Helper Functions

```glados
fn add(a, b) {
    return a + b
}

fn multiply(a, b) {
    return a * b
}

fn subtract(a, b) {
    return a - b
}

fn calculate() {
    x = add(5, 10)
    y = multiply(x, 3)
    z = subtract(y, 5)
    return z
}

fn main() {
    return calculate()
}
```

**Output**: `40` ((5+10)*3 - 5)

## Mutual Recursion

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
    return is_even(42)
}
```

**Output**: `1` (true)

## Next Steps

- See [advanced examples](./advanced.md) with complex algorithms
- Learn about [functions](../language/functions.md) in detail
