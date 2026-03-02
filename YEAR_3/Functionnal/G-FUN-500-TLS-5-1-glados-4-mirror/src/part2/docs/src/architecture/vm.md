# Virtual Machine

The GLaDOS Virtual Machine (VM) executes compiled bytecode using a stack-based execution model.

## Overview

The VM is implemented in `src/part2/VM/VM.hs` and provides a runtime environment for executing GLaDOS programs.

## VM Architecture

```text
┌─────────────────────────────────┐
│         VM State                │
├─────────────────────────────────┤
│  • Stack (operand stack)        │
│  • Variables (local vars)       │
│  • Functions (function table)   │
│  • PC (program counter)         │
└─────────────────────────────────┘
```

## VM State

```haskell
data VMState = VMState
    { stack :: [Value]                                    -- Operand stack
    , vars :: Map.Map String Value                        -- Variables
    , functions :: Map.Map String ([String], [Instruction])  -- Functions
    , pc :: Int                                           -- Program counter
    }
```

### Components

**Stack**: Holds intermediate values during computation
**Variables**: Maps variable names to values
**Functions**: Maps function names to (parameters, bytecode)
**PC**: Points to the current instruction

## Execution Model

### Main Execution Loop

```haskell
runVM :: [Instruction] -> VMState -> Either String VMState
runVM instructions state
    | pc state >= length instructions = Right state
    | otherwise = do
        let instr = instructions !! pc state
        newState <- executeInstruction instr instructions state
        runVM instructions newState
```

The VM:
1. Fetches the instruction at PC
2. Executes the instruction
3. Updates PC
4. Repeats until PC reaches end

### Instruction Execution

```haskell
executeInstruction :: Instruction -> [Instruction] -> VMState -> Either String VMState
```

Each instruction updates the VM state and advances the PC.

## Stack Operations

### Push

Pushes a value onto the stack:

```
Stack: [...]
Execute: Push (VInt 42)
Stack: [42, ...]
```

### Pop

Removes the top value:

```
Stack: [42, ...]
Execute: Pop
Stack: [...]
```

## Variable Operations

### LoadVar

Loads a variable onto the stack:

```
Vars: {x → 42}
Stack: [...]
Execute: LoadVar "x"
Stack: [42, ...]
```

### StoreVar

Stores top of stack in a variable:

```
Stack: [42, ...]
Execute: StoreVar "x"
Stack: [...]
Vars: {x → 42}
```

## Binary Operations

Binary operations pop two operands, compute, and push the result:

```
Stack: [20, 10, ...]    // Right operand on top
Execute: BinOp Add
Stack: [30, ...]
```

### Supported Operations

- **Arithmetic**: Add, Sub, Mul, Div
- **Comparison**: Eq, NotEq, Less, Greater, LessEq, GreaterEq

## Control Flow

### JumpIfFalse

Conditional jump based on stack top:

```
Stack: [false, ...]
Execute: JumpIfFalse 5
Stack: [...]
PC: PC + 5 + 1          // Jump forward
```

If true or non-zero:
```
Stack: [true, ...]
Execute: JumpIfFalse 5
Stack: [...]
PC: PC + 1              // Continue normally
```

### Jump

Unconditional jump:

```
Execute: Jump 10
PC: PC + 10 + 1         // Jump forward
```

## Function Calls

### CallFunc

1. Pop arguments from stack (in reverse order)
2. Look up function in function table
3. Bind arguments to parameters
4. Execute function bytecode
5. Push return value onto stack

```
Stack: [20, 10, ...]
Execute: CallFunc "add" 2
Stack: [30, ...]        // Result of add(10, 20)
```

### Return

Returns from a function with the value on top of stack.

## Type System

Values are tagged with types:

```haskell
data Value =
    VInt Int
    | VBool Bool
```

### Type Checking

Type checking happens at runtime:

```haskell
applyOp :: Op -> Value -> Value -> Either String Value
applyOp Add (VInt a) (VInt b) = Right (VInt (a + b))
applyOp Add _ _ = Left "Type error: Add requires integers"
```

## Error Handling

The VM can fail with errors:

- **Stack underflow**: Not enough operands
- **Type error**: Invalid operation for types
- **Variable not found**: Undefined variable
- **Function not found**: Undefined function
- **Division by zero**: Dividing by zero

Example:
```
Error: Variable not found: x
Error: Type error: Cannot add boolean to integer
Error: Division by zero
```

## Execution Example

Source:
```glados
fn main() {
    x = 10
    y = 20
    return x + y
}
```

Bytecode:
```
[CallFunc "main" 0, Return]
```

Function `main`:
```
Push (VInt 10)
StoreVar "x"
Push (VInt 20)
StoreVar "y"
LoadVar "x"
LoadVar "y"
BinOp Add
Return
```

Execution trace:
```
PC=0: Push (VInt 10)      → Stack: [10]
PC=1: StoreVar "x"        → Stack: [], Vars: {x→10}
PC=2: Push (VInt 20)      → Stack: [20]
PC=3: StoreVar "y"        → Stack: [], Vars: {x→10, y→20}
PC=4: LoadVar "x"         → Stack: [10]
PC=5: LoadVar "y"         → Stack: [20, 10]
PC=6: BinOp Add           → Stack: [30]
PC=7: Return              → Returns 30
```

## Performance

The VM is an interpreter with reasonable performance for educational purposes:

- **Stack operations**: O(1)
- **Variable lookup**: O(log n) with Map
- **Function lookup**: O(log n) with Map

## Debugging

Run with `-v` flag for verbose output:

```bash
./glados-vm -v program.bc
```

Shows:
- Each instruction executed
- Stack state
- Variable bindings
- Function calls

## Next Steps

- Learn about the [bytecode format](./bytecode.md)
- Understand the [instruction set](./instructions.md)
- Explore the [execution model](./execution.md) in detail
