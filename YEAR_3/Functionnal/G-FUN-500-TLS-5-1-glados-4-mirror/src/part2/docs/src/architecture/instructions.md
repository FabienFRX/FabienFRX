# Instruction Set

The GLaDOS VM uses a stack-based instruction set for bytecode execution.

## Instruction Reference

### Stack Operations

#### Push

```haskell
Push Value
```

Pushes a value onto the stack.

**Example**:
```
Push (VInt 42)      -- Push integer 42
Push (VBool True)   -- Push boolean true
```

**Effect**:
- Stack before: `[...]`
- Stack after: `[value, ...]`
- PC: `PC + 1`

#### Pop

```haskell
Pop
```

Removes the top value from the stack.

**Effect**:
- Stack before: `[value, ...]`
- Stack after: `[...]`
- PC: `PC + 1`

### Variable Operations

#### LoadVar

```haskell
LoadVar String
```

Loads a variable's value onto the stack.

**Example**:
```
LoadVar "x"
```

**Effect**:
- Stack: `[vars[name], ...]`
- PC: `PC + 1`
- Error if variable not found

#### StoreVar

```haskell
StoreVar String
```

Stores the top of stack in a variable.

**Example**:
```
StoreVar "x"
```

**Effect**:
- Stack before: `[value, ...]`
- Stack after: `[...]`
- Variables: `vars[name] = value`
- PC: `PC + 1`

### Binary Operations

#### BinOp

```haskell
BinOp Op
```

Performs a binary operation on the top two stack values.

**Operators**:
- `Add`, `Sub`, `Mul`, `Div` (arithmetic)
- `Eq`, `NotEq`, `Less`, `Greater`, `LessEq`, `GreaterEq` (comparison)

**Example**:
```
BinOp Add
```

**Effect**:
- Stack before: `[right, left, ...]`
- Stack after: `[result, ...]`
- PC: `PC + 1`
- Error on type mismatch or division by zero

**Operations**:

| Operation | Left | Right | Result | Description |
|-----------|------|-------|--------|-------------|
| Add | Int | Int | Int | Addition |
| Sub | Int | Int | Int | Subtraction |
| Mul | Int | Int | Int | Multiplication |
| Div | Int | Int | Int | Division (truncated) |
| Eq | Int | Int | Bool | Equality |
| NotEq | Int | Int | Bool | Inequality |
| Less | Int | Int | Bool | Less than |
| Greater | Int | Int | Bool | Greater than |
| LessEq | Int | Int | Bool | Less or equal |
| GreaterEq | Int | Int | Bool | Greater or equal |

### Control Flow

#### JumpIfFalse

```haskell
JumpIfFalse Int
```

Conditional jump: jumps forward if top of stack is false or zero.

**Example**:
```
JumpIfFalse 5
```

**Effect**:
- Stack before: `[condition, ...]`
- Stack after: `[...]`
- If false/zero: `PC = PC + offset + 1`
- If true/non-zero: `PC = PC + 1`

**Truthiness**:
- `VBool False` → jump
- `VBool True` → don't jump
- `VInt 0` → jump
- `VInt n` (n ≠ 0) → don't jump

#### Jump

```haskell
Jump Int
```

Unconditional jump forward.

**Example**:
```
Jump 10
```

**Effect**:
- PC: `PC + offset + 1`

### Function Operations

#### CallFunc

```haskell
CallFunc String Int
```

Calls a function with N arguments.

**Example**:
```
CallFunc "add" 2
```

**Effect**:
1. Pops N arguments from stack (in reverse order)
2. Looks up function in function table
3. Binds arguments to parameters
4. Executes function body
5. Pushes return value onto stack
6. PC: `PC + 1`

**Errors**:
- Function not found
- Argument count mismatch

#### Return

```haskell
Return
```

Returns from a function with the value on top of stack.

**Effect**:
- Stack before: `[return_value, ...]`
- Returns `return_value` to caller
- In main context: terminates program

## Instruction Summary Table

| Instruction | Operands | Stack Effect | Description |
|-------------|----------|--------------|-------------|
| `Push` | Value | `[] → [v]` | Push value |
| `Pop` | - | `[v] → []` | Pop value |
| `LoadVar` | String | `[] → [value]` | Load variable |
| `StoreVar` | String | `[v] → []` | Store variable |
| `BinOp` | Op | `[r, l] → [result]` | Binary operation |
| `JumpIfFalse` | Int | `[cond] → []` | Conditional jump |
| `Jump` | Int | - | Unconditional jump |
| `CallFunc` | String, Int | `[args...] → [result]` | Call function |
| `Return` | - | `[v] → returns v` | Return from function |

## Example Instruction Sequences

### Simple Addition

Source: `10 + 20`

Bytecode:
```
Push (VInt 10)
Push (VInt 20)
BinOp Add
```

Execution:
```
PC=0: Push 10    → Stack: [10]
PC=1: Push 20    → Stack: [20, 10]
PC=2: BinOp Add  → Stack: [30]
```

### If-Else

Source: `if (x > 0) return 1 else return 0`

Bytecode:
```
LoadVar "x"
Push (VInt 0)
BinOp Greater
JumpIfFalse 2
Push (VInt 1)
Return
Jump 2
Push (VInt 0)
Return
```

### Function Call

Source: `add(10, 20)`

Bytecode:
```
Push (VInt 10)
Push (VInt 20)
CallFunc "add" 2
```

## Instruction Encoding

Instructions are encoded in binary format:

```
┌─────────────────┬──────────────────────┐
│ Tag (1 byte)    │ Data (variable)      │
├─────────────────┼──────────────────────┤
│ 0: Push         │ Value                │
│ 1: Pop          │ -                    │
│ 2: LoadVar      │ String               │
│ 3: StoreVar     │ String               │
│ 4: BinOp        │ Op tag               │
│ 5: JumpIfFalse  │ Int                  │
│ 6: Jump         │ Int                  │
│ 7: CallFunc     │ String, Int          │
│ 8: Return       │ -                    │
└─────────────────┴──────────────────────┘
```

## Error Conditions

| Instruction | Error Conditions |
|-------------|------------------|
| `Push` | None |
| `Pop` | Stack underflow |
| `LoadVar` | Variable not found |
| `StoreVar` | Stack underflow |
| `BinOp` | Stack underflow, type error, division by zero |
| `JumpIfFalse` | Stack underflow |
| `Jump` | None |
| `CallFunc` | Function not found, stack underflow, arg count mismatch |
| `Return` | Stack underflow |

## Next Steps

- Learn about the [VM](./vm.md) that executes these instructions
- Understand the [execution model](./execution.md)
- See the [bytecode format](./bytecode.md)
