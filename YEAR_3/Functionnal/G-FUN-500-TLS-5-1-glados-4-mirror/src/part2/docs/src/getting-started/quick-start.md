# Quick Start

This guide will help you get up and running with GLaDOS in just a few minutes.

## Prerequisites

Before you begin, make sure you have:

- GHC (Glasgow Haskell Compiler) installed
- Cabal build system
- Basic familiarity with command-line tools

## Building GLaDOS

1. Navigate to the project root:
   ```bash
   cd /path/to/glados/src/part2
   ```

2. Build the project using Make:
   ```bash
   cd Build
   make
   ```

   This will compile both the compiler and the VM executables:
   - `glados-compiler` - The GLaDOS compiler
   - `glados-vm` - The GLaDOS virtual machine

## Your First GLaDOS Program

1. Create a file named `hello.gld`:
   ```glados
   fn main() {
       return 42
   }
   ```

2. Compile it:
   ```bash
   ./glados-compiler -o hello.bc hello.gld
   ```

3. Run it:
   ```bash
   ./glados-vm hello.bc
   ```

   Output:
   ```
   42
   ```

## A More Complex Example

Here's a factorial function:

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

Compile and run:
```bash
./glados-compiler -o factorial.bc factorial.gld
./glados-vm factorial.bc
```

Output:
```
120
```

## Running with Verbose Output

To see detailed execution information, use the `-v` flag:

```bash
./glados-vm -v factorial.bc
```

This will show:
- Stack state at each step
- Variable bindings
- Function calls and returns
- Instruction execution trace

## Exploring Examples

The `examples/` directory contains many example programs demonstrating various features:

```bash
# Run all examples
for file in ../examples/*.gld; do
    echo "Running: $file"
    ./glados-compiler -o test.bc "$file" && ./glados-vm test.bc
    echo "---"
done
```

## Next Steps

- Learn about [GLaDOS syntax](../language/syntax.md)
- Understand [data types](../language/data-types.md)
- Explore [functions and recursion](../language/functions.md)
- Read about the [compiler architecture](../architecture/compiler.md)
