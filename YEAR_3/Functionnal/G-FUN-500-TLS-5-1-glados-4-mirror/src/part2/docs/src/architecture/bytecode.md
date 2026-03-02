# Bytecode Format

The bytecode format defines how compiled GLaDOS programs are stored and loaded.

## Overview

GLaDOS bytecode is a binary format containing compiled instructions and function definitions.

**Location**: `src/part2/VM/CompilerInterface.hs`

## Bytecode Structure

```haskell
data Bytecode = Bytecode
    { entryPoint :: [Instruction]                              -- Main entry code
    , functions :: Map.Map String ([String], [Instruction])    -- Function table
    }
```

### Components

**Entry Point**: Instructions that start execution (calls `main()`)
**Functions**: Map of function names to (parameters, instructions)

## Binary Serialization

Bytecode is serialized using Haskell's `Binary` library:

```haskell
instance Binary Bytecode where
    put (Bytecode entry funcs) = do
        put entry
        put (Map.toList funcs)

    get = do
        entry <- get
        funcList <- get
        return $ Bytecode entry (Map.fromList funcList)
```

## File Format

### Structure

```
┌──────────────────────────────┐
│  Entry Point Instructions    │
│  (serialized list)            │
├──────────────────────────────┤
│  Function Count (Int)         │
├──────────────────────────────┤
│  Function 1:                  │
│    • Name (String)            │
│    • Parameters ([String])    │
│    • Instructions ([Instr])   │
├──────────────────────────────┤
│  Function 2:                  │
│    • ...                      │
├──────────────────────────────┤
│  ...                          │
└──────────────────────────────┘
```

### Example

For this program:
```glados
fn add(a, b) {
    return a + b
}

fn main() {
    return add(10, 20)
}
```

Bytecode contains:
- Entry point: `[CallFunc "main" 0, Return]`
- Functions:
  - `"add"` → `(["a", "b"], [LoadVar "a", LoadVar "b", BinOp Add, Return])`
  - `"main"` → `([], [Push 10, Push 20, CallFunc "add" 2, Return])`

## Loading Bytecode

```haskell
loadBytecode :: FilePath -> IO (Either String Bytecode)
```

Reads a `.bc` file and deserializes it into a `Bytecode` structure.

## Saving Bytecode

```haskell
saveBytecode :: FilePath -> Bytecode -> IO ()
```

Serializes a `Bytecode` structure and writes it to a `.bc` file.

## Instruction Encoding

Each instruction type is encoded as a tag followed by its data:

```haskell
instance Binary Instruction where
    put (Push val) = putWord8 0 >> put val
    put Pop = putWord8 1
    put (LoadVar name) = putWord8 2 >> put name
    put (StoreVar name) = putWord8 3 >> put name
    -- ... etc
```

## Value Encoding

```haskell
instance Binary Value where
    put (VInt n) = putWord8 0 >> put n
    put (VBool b) = putWord8 1 >> put b

    get = do
        tag <- getWord8
        case tag of
            0 -> VInt <$> get
            1 -> VBool <$> get
```

## File Extension

Bytecode files use the `.bc` extension by convention:
- `program.gld` → source code
- `program.bc` → compiled bytecode

## Portability

The bytecode format is:
- **Platform-independent**: Binary serialization handles endianness
- **Version-independent** (currently): No versioning yet
- **Compact**: Binary format is smaller than source

## Bytecode Verification

When loading bytecode, the VM verifies:
- `main()` function exists
- All called functions are defined
- Bytecode is well-formed

## Pretty Printing

For debugging, bytecode can be printed in human-readable form:

```haskell
printBytecode :: Bytecode -> IO ()
```

Output:
```
Entry Point:
  CallFunc "main" 0
  Return

Functions:
  add(a, b):
    LoadVar "a"
    LoadVar "b"
    BinOp Add
    Return

  main():
    Push (VInt 10)
    Push (VInt 20)
    CallFunc "add" 2
    Return
```

## Future Enhancements

Potential improvements:
- Version numbers for compatibility
- Compression
- Debug symbols (line numbers, source maps)
- Checksums for integrity
- Optimization metadata

## Next Steps

- Understand the [instruction set](./instructions.md)
- Learn about the [VM](./vm.md) execution
- Explore the [execution model](./execution.md)
