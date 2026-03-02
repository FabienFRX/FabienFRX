# Code Generation

Code generation transforms the Abstract Syntax Tree (AST) into bytecode instructions that the VM can execute.

## Overview

The code generator is part of the compiler (`src/part2/Compiler/Compiler.hs`) and implements a straightforward AST-to-bytecode translation.

## Code Generation Strategy

GLaDOS uses a **stack-based code generation** approach:
- Operands are pushed onto a stack
- Operations consume operands from the stack
- Results are pushed back onto the stack

## Compilation Rules

### Literals

**Integers**:
```haskell
AInt n  →  [Push (VInt n)]
```

Example:
```glados
42  →  [Push (VInt 42)]
```

**Booleans**:
```haskell
ABool b  →  [Push (VBool b)]
```

### Variables

**Load Variable**:
```haskell
AVar name  →  [LoadVar name]
```

Example:
```glados
x  →  [LoadVar "x"]
```

**Store Variable**:
```haskell
AAssign name expr  →  compile(expr) ++ [StoreVar name]
```

Example:
```glados
x = 42  →  [Push (VInt 42), StoreVar "x"]
```

### Binary Operations

```haskell
ABinOp left right  →  compile(left) ++ compile(right) ++ [BinOp op]
```

Example:
```glados
10 + 20  →  [Push (VInt 10), Push (VInt 20), BinOp Add]
```

Result: Stack has the sum (30)

### Conditionals

```haskell
AIf cond thenBranch elseBranch  →
    compile(cond) ++
    [JumpIfFalse (length(thenCode) + 1)] ++
    compile(thenBranch) ++
    [Jump (length(elseCode))] ++
    compile(elseBranch)
```

Example:
```glados
if (x > 0) {
    return 1
} else {
    return 0
}

→ [LoadVar "x", Push (VInt 0), BinOp Greater,
   JumpIfFalse 2,
   Push (VInt 1), Return,
   Jump 2,
   Push (VInt 0), Return]
```

### Function Calls

```haskell
ACall name args  →
    compile(args[0]) ++ ... ++ compile(args[n]) ++
    [CallFunc name (length args)]
```

Example:
```glados
add(10, 20)  →  [Push (VInt 10), Push (VInt 20), CallFunc "add" 2]
```

### Return Statements

```haskell
AReturn expr  →  compile(expr) ++ [Return]
```

Example:
```glados
return 42  →  [Push (VInt 42), Return]
```

## Complete Examples

### Simple Arithmetic

Source:
```glados
fn main() {
    x = 10
    y = 20
    result = x + y
    return result
}
```

Bytecode:
```
Push (VInt 10)
StoreVar "x"
Push (VInt 20)
StoreVar "y"
LoadVar "x"
LoadVar "y"
BinOp Add
StoreVar "result"
LoadVar "result"
Return
```

### Factorial Function

Source:
```glados
fn factorial(n) {
    if (n < 2) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}
```

Bytecode:
```
LoadVar "n"
Push (VInt 2)
BinOp Less
JumpIfFalse 2         // Skip to else branch
Push (VInt 1)
Return
Jump 7                // Skip else branch
LoadVar "n"
LoadVar "n"
Push (VInt 1)
BinOp Sub
CallFunc "factorial" 1
BinOp Mul
Return
```

## Optimization Opportunities

The current code generator performs no optimization. Future optimizations could include:

### Constant Folding

```glados
x = 2 + 3  →  x = 5
```

Current:
```
Push (VInt 2)
Push (VInt 3)
BinOp Add
StoreVar "x"
```

Optimized:
```
Push (VInt 5)
StoreVar "x"
```

### Dead Code Elimination

```glados
if (true) {
    return 1
} else {
    return 0  // Never executed
}
```

### Tail Call Optimization

```glados
fn factorial(n, acc) {
    if (n < 2) {
        return acc
    } else {
        return factorial(n - 1, n * acc)  // Tail call
    }
}
```

Could be optimized to a loop in bytecode.

## Implementation Details

### Code Generation Function

```haskell
compileAST :: AST -> Either String [Instruction]
```

Returns either an error or a list of instructions.

### Handling Errors

```haskell
compileAST (ADiv left right) = do
    leftCode <- compileAST left
    rightCode <- compileAST right
    return $ leftCode ++ rightCode ++ [BinOp Div]
```

Uses Haskell's `Either` monad for error propagation.

### Function Compilation

```haskell
compileFunction :: AST -> Either String (String, ([String], [Instruction]))
compileFunction (AFuncDef name params body) = do
    bodyCode <- compileAST body
    let codeWithReturn = bodyCode ++ [Return]
    return (name, (params, codeWithReturn))
```

Each function is compiled separately and stored in a function table.

## Jump Offset Calculation

### Forward Jumps

For `JumpIfFalse` and `Jump`, offsets are calculated based on code length:

```haskell
let thenLen = length thenCode
    elseLen = length elseCode

return $ condCode ++
         [JumpIfFalse (thenLen + 1)] ++  -- Skip then block + the Jump instruction
         thenCode ++
         [Jump elseLen] ++                -- Skip else block
         elseCode
```

### Offset Semantics

- `JumpIfFalse n`: Skip `n` instructions forward
- `Jump n`: Skip `n` instructions forward
- Offsets are relative to the current instruction

## Stack Management

The code generator assumes:
- Values are pushed before operations
- Operations pop operands and push results
- Variables are loaded/stored via dedicated instructions
- Function calls clean up their arguments

## Type Handling

Type checking happens at runtime in the VM. The code generator:
- Doesn't perform type checking
- Generates code assuming valid types
- Relies on VM for type errors

## Next Steps

- Understand the [VM](./vm.md) that executes the bytecode
- Learn about the [instruction set](./instructions.md)
- See the [bytecode format](./bytecode.md)
- Explore the [execution model](./execution.md)
