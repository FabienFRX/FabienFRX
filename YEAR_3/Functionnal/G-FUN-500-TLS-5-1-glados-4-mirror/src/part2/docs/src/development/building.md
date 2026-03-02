# Building from Source

This guide covers building GLaDOS from source code.

## Prerequisites

- GHC 8.10 or later
- Cabal 3.0 or later
- Make (optional, for convenience)
- Git (for development)

## Quick Build

```bash
cd src/part2/Build
make
```

This builds everything and runs tests.

## Build System

GLaDOS uses Cabal as its build system, with a Makefile for convenience.

### Project Structure

```
part2/
├── glados.cabal        # Cabal package definition (in root)
├── Build/
│   └── Makefile        # Build automation
├── Compiler/           # Compiler source
├── VM/                 # VM source
├── App/                # Executables
└── tests/              # Test suite
```

## Makefile Targets

### `make` or `make all`

Builds library, executables, and runs tests.

```bash
make
```

### `make build`

Only builds without running tests.

```bash
make build
```

### `make test`

Runs the test suite.

```bash
make test
```

### `make clean`

Removes build artifacts.

```bash
make clean
```

### `make rebuild`

Clean and rebuild from scratch.

```bash
make rebuild
```

## Manual Building

If you prefer not to use Make:

### Build Library

```bash
cabal build glados
```

### Build Compiler

```bash
cabal build glados:exe:glados
```

### Build VM

```bash
cabal build glados:exe:glados-vm
```

### Run Tests

```bash
cabal test
```

## Build Output

After building, executables are located in:

```
Build/
├── glados-compiler     # Compiler executable
└── glados-vm           # VM executable
```

## Development Build

For development with automatic recompilation:

```bash
cabal build --enable-optimization=0
```

This disables optimizations for faster builds.

## Release Build

For optimized release builds:

```bash
cabal build --enable-optimization=2
```

## Dependencies

GLaDOS depends on these Haskell packages:

- `base` - Standard library
- `containers` - Data structures (Map)
- `parsec` - Parser combinators
- `binary` - Binary serialization
- `bytestring` - Byte strings
- `filepath` - File path handling
- `hspec` - Testing framework (dev only)

### Installing Dependencies

```bash
cabal update
cabal install --only-dependencies
```

## Troubleshooting

### Build Fails

Try cleaning and rebuilding:

```bash
cabal clean
rm -rf dist-newstyle
cabal build
```

### Missing Dependencies

Update package index:

```bash
cabal update
```

### GHC Version Issues

Check GHC version:

```bash
ghc --version
```

Upgrade if needed using GHCup.

### Cabal Cache Issues

Clear Cabal cache:

```bash
rm -rf ~/.cabal/store
cabal update
```

## Build Configuration

### Compiler Flags

Edit `glados.cabal` to change compiler flags:

```cabal
ghc-options: -Wall -Wcompat -Widentities ...
```

### Optimization Levels

- `-O0`: No optimization (fast builds)
- `-O1`: Basic optimization
- `-O2`: Full optimization (default for release)

## Cross-Platform Building

### Linux

Works out of the box.

### macOS

```bash
brew install ghc cabal-install
cabal build
```

### Windows (WSL)

Use Windows Subsystem for Linux:

```bash
sudo apt update
sudo apt install ghc cabal-install
cabal build
```

## Next Steps

- Learn about [testing](./testing.md)
- Read [contributing guidelines](./contributing.md)
