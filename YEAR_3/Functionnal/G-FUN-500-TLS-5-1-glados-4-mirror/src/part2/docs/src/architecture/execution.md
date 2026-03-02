# Execution Model

This page describes how GLaDOS programs are executed by the virtual machine.

## Execution Flow

```text
1. Load bytecode file (.bc)
2. Initialize VM state
3. Execute entry point
4. Call main()
5. Execute main() bytecode
6. Return result
```

## VM Initialization

```haskell
initVM :: VMState
initVM = VMState
    { stack = []
    , vars = Map.empty
    , functions = Map.empty
    , pc = 0
    }
```

Initial state:
- Empty stack
- No variables
- No functions (loaded from bytecode)
- PC at 0

## Program Execution

### Entry Point

Every program starts with entry point code:

```
[CallFunc "main" 0, Return]
```

This calls `main()` and returns its result.

### Function Execution

When a function is called:

1. **Argument Binding**: Pop arguments from stack and bind to parameters
2. **Execution**: Execute function bytecode with PC=0
3. **Return**: Push return value onto caller's stack
4. **Resume**: Continue caller's execution

### Example Execution

Source:
```glados
fn add(a, b) {
    return a + b
}

fn main() {
    result = add(10, 20)
    return result
}
```

Execution trace:
```
[Entry Point]
PC=0: CallFunc "main" 0

[main()]
PC=0: Push (VInt 10)          Stack: [10]
PC=1: Push (VInt 20)          Stack: [20, 10]
PC=2: CallFunc "add" 2        → Call add(10, 20)

[add(a=10, b=20)]
PC=0: LoadVar "a"             Stack: [10]
PC=1: LoadVar "b"             Stack: [20, 10]
PC=2: BinOp Add               Stack: [30]
PC=3: Return                  → Return 30

[main() resumed]
PC=3: StoreVar "result"       Vars: {result=30}, Stack: []
PC=4: LoadVar "result"        Stack: [30]
PC=5: Return                  → Return 30

[Entry Point resumed]
PC=1: Return                  → Program result: 30
```

## Stack Execution Model

GLaDOS uses a **stack-based** execution model:

### Stack Operations

```
Push 10        [10]
Push 20        [20, 10]
BinOp Add      [30]
```

Values are pushed and popped in LIFO order.

### Expression Evaluation

Expressions are evaluated in postfix order:

```glados
(2 + 3) * 4
```

Bytecode:
```
Push 2
Push 3
BinOp Add      → [5]
Push 4         → [4, 5]
BinOp Mul      → [20]
```

## Variable Scoping

### Local Variables

Variables are function-scoped:

```glados
fn func1() {
    x = 10
    return x
}

fn func2() {
    x = 20    // Different x
    return x
}
```

Each function has its own variable map.

### Global Variables

There are no global variables. All variables are local to their function.

## Function Calls

### Call Stack (Implicit)

While the VM doesn't explicitly maintain a call stack, recursion works through Haskell's native call stack.

### Argument Passing

Arguments are passed by value:

```glados
fn modify(x) {
    x = 100
    return x
}

fn main() {
    y = 42
    result = modify(y)
    return y    // Still 42
}
```

### Return Values

Functions must return a value:

```glados
fn get_answer() {
    return 42
}
```

The return value is pushed onto the caller's stack.

## Control Flow

### Conditional Execution

```glados
if (x > 0) {
    return 1
} else {
    return 0
}
```

Bytecode:
```
LoadVar "x"
Push 0
BinOp Greater
JumpIfFalse 2    // If false, skip "then" branch
Push 1
Return
Jump 2           // Skip "else" branch
Push 0
Return
```

Execution:
- If `x > 0` is true: Execute first return
- If `x > 0` is false: Jump to else, execute second return

### Recursion

Recursion works naturally:

```glados
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}
```

Each recursive call gets its own variable scope.

## Type System

### Runtime Type Checking

Types are checked at runtime during operations:

```
BinOp Add:
  VInt + VInt  → VInt     ✓
  VBool + VInt → Error    ✗
```

### Type Coercion

In conditionals, integers can be used as booleans:
- `0` → false
- Non-zero → true

```glados
if (42) {       // 42 is truthy
    return 1
}
```

## Error Handling

Errors stop execution and return an error message:

```
Error: Variable not found: x
Error: Type error: Cannot add boolean to integer
Error: Division by zero
Error: Stack underflow
Error: Function not found: foo
```

## Performance Characteristics

### Time Complexity

- **Instruction execution**: O(1) per instruction
- **Variable lookup**: O(log n) with Map
- **Function lookup**: O(log n) with Map

### Space Complexity

- **Stack**: O(expression depth)
- **Variables**: O(number of variables)
- **Functions**: O(number of functions)

## Execution Modes

### Normal Mode

```bash
./glados-vm program.bc
```

Executes and prints the result.

### Verbose Mode

```bash
./glados-vm -v program.bc
```

Shows detailed execution trace:
```
Executing: Push (VInt 10)
Stack: [10]

Executing: Push (VInt 20)
Stack: [20, 10]

Executing: BinOp Add
Stack: [30]
```

## Execution Guarantees

- **Deterministic**: Same input always produces same output
- **Type-safe**: Type errors are caught at runtime
- **Memory-safe**: No buffer overflows or memory corruption (Haskell guarantees)

## Termination

Program terminates when:
- Entry point executes `Return`
- An error occurs
- Stack becomes empty (implicit return 0)

## Next Steps

- Understand [bytecode format](./bytecode.md)
- Learn about the [instruction set](./instructions.md)
- Explore the [VM implementation](./vm.md)
