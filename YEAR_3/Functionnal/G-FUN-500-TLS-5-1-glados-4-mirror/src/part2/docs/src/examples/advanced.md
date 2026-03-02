# Advanced Examples

Complex algorithms and advanced programming patterns in GLaDOS.

## Greatest Common Divisor (GCD)

Euclidean algorithm using modulo:

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
    return gcd(48, 18)
}
```

**Output**: `6`

## Ackermann Function

A computationally intensive recursive function:

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
    return ackermann(3, 3)
}
```

**Output**: `61`

## Collatz Conjecture

Count steps to reach 1:

```glados
fn is_even_div(n) {
    return n - (n / 2) * 2 == 0
}

fn collatz_step(n) {
    if (is_even_div(n)) {
        return n / 2
    } else {
        return n * 3 + 1
    }
}

fn collatz_count(n) {
    if (n <= 1) {
        return 0
    } else {
        return 1 + collatz_count(collatz_step(n))
    }
}

fn main() {
    return collatz_count(27)
}
```

**Output**: `111`

## Boolean Logic Implementation

```glados
fn bool_to_int(b) {
    if (b) { return 1 } else { return 0 }
}

fn and(a, b) {
    if (a) {
        if (b) { return 1 } else { return 0 }
    } else {
        return 0
    }
}

fn or(a, b) {
    if (a) {
        return 1
    } else {
        if (b) { return 1 } else { return 0 }
    }
}

fn not(b) {
    if (b) { return 0 } else { return 1 }
}

fn xor(a, b) {
    return and(or(a, b), not(and(a, b)))
}

fn main() {
    x = 10 > 5
    y = 3 < 2

    result = xor(x, y)
    return bool_to_int(result)
}
```

**Output**: `1` (true XOR false = true)

## Range Checking with Guards

```glados
fn in_range(value, min_val, max_val) {
    if (value < min_val) {
        return 0
    } else {
        if (value > max_val) {
            return 0
        } else {
            return 1
        }
    }
}

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
    a = in_range(15, 10, 20)
    b = clamp(25, 10, 20)
    return a + b
}
```

**Output**: `21` (1 + 20)

## Prime Number Checking

```glados
fn mod(a, b) {
    return a - (a / b) * b
}

fn is_divisible(n, d) {
    return mod(n, d) == 0
}

fn has_divisor_from(n, d) {
    if (d * d > n) {
        return 0
    } else {
        if (is_divisible(n, d)) {
            return 1
        } else {
            return has_divisor_from(n, d + 1)
        }
    }
}

fn is_prime(n) {
    if (n < 2) {
        return 0
    } else {
        return not(has_divisor_from(n, 2))
    }
}

fn not(b) {
    if (b) { return 0 } else { return 1 }
}

fn main() {
    return is_prime(17)
}
```

**Output**: `1` (true, 17 is prime)

## Tower of Hanoi Moves

Calculate number of moves needed:

```glados
fn hanoi_moves(n) {
    if (n <= 0) {
        return 0
    } else {
        if (n == 1) {
            return 1
        } else {
            prev = hanoi_moves(n - 1)
            return 2 * prev + 1
        }
    }
}

fn main() {
    return hanoi_moves(5)
}
```

**Output**: `31` (2^5 - 1)

## Next Steps

- Review [language features](../language/overview.md)
- Understand [recursion patterns](../language/functions.md)
- Explore the [VM architecture](../architecture/vm.md)
