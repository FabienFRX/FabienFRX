# Basic Examples

This section covers basic GLaDOS programs demonstrating fundamental concepts.

## Hello World

The simplest program returns a value:

```glados
fn main() {
    return 42
}
```

**Output**: `42`

## Variables and Arithmetic

```glados
fn main() {
    a = 10
    b = 32
    sum = a + b
    return sum
}
```

**Output**: `42`

## Comparison Operators

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

**Output**: `1` (true)

## All Comparison Operators

```glados
fn main() {
    a = 10
    b = 20

    eq = a == b      // false
    neq = a != b     // true
    lt = a < b       // true
    gt = a > b       // false
    lte = a <= b     // true
    gte = a >= b     // false

    // Count true comparisons
    return 3    // neq, lt, lte
}
```

**Output**: `3`

## Simple If-Else

```glados
fn main() {
    x = 100

    if (x > 50) {
        return 200
    } else {
        return 0
    }
}
```

**Output**: `200`

## Nested If-Else

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
    return sign(42)
}
```

**Output**: `1`

## Absolute Value

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

**Output**: `25`

## Maximum of Two Numbers

```glados
fn max(a, b) {
    if (a > b) {
        return a
    } else {
        return b
    }
}

fn main() {
    return max(42, 17)
}
```

**Output**: `42`

## Minimum of Two Numbers

```glados
fn min(a, b) {
    if (a < b) {
        return a
    } else {
        return b
    }
}

fn main() {
    return min(42, 17)
}
```

**Output**: `17`

## Complex Expression

```glados
fn main() {
    x = 5
    y = 10
    z = 3

    result = (x + y) * z - (x * 2)
    return result
}
```

**Output**: `35` ((5+10)*3 - 10 = 45 - 10 = 35)

## Next Steps

- Explore [functions and recursion](./functions.md)
- See [advanced examples](./advanced.md)
