# Installation

This guide covers how to install and set up the GLaDOS development environment.

## System Requirements

GLaDOS requires:

- **Operating System**: Linux, macOS, or Windows (with WSL)
- **GHC**: Version 8.10 or later
- **Cabal**: Version 3.0 or later
- **Disk Space**: Approximately 500MB for dependencies and build artifacts
- **RAM**: At least 2GB for compilation

## Installing Haskell

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install ghc cabal-install
cabal update
```

### macOS

Using Homebrew:
```bash
brew install ghc cabal-install
cabal update
```

### Using GHCup (Recommended)

The recommended way to install Haskell on any platform:

```bash
curl --proto '=https' --tlsv1.2 -sSf https://get-ghcup.haskell.org | sh
```

Follow the prompts to install GHC and Cabal.

## Building GLaDOS

1. Clone the repository (or navigate to your project directory):
   ```bash
   cd /path/to/glados
   ```

2. Navigate to the Part 2 build directory:
   ```bash
   cd src/part2/Build
   ```

3. Build using Make:
   ```bash
   make
   ```

   This will:
   - Install all Haskell dependencies
   - Compile the GLaDOS library
   - Build the compiler executable
   - Build the VM executable
   - Run tests

## Build Targets

The Makefile provides several useful targets:

### `make` or `make all`
Builds everything (library, executables, and runs tests)

### `make build`
Only builds the library and executables without running tests

### `make test`
Runs the test suite

### `make clean`
Removes all build artifacts

### `make rebuild`
Cleans and rebuilds everything

## Verifying Installation

After building, verify that the executables work:

```bash
# Check compiler
./glados-compiler --help

# Check VM
./glados-vm --help
```

You should see the help messages for each tool.

## Project Structure

After building, your directory structure will look like:

```
part2/
├── Build/               # Build output directory
│   ├── glados-compiler  # Compiler executable
│   ├── glados-vm        # VM executable
│   └── Makefile         # Build configuration
├── Compiler/            # Compiler source code
│   ├── AST.hs          # Abstract Syntax Tree
│   ├── Parser.hs       # Parser implementation
│   └── Compiler.hs     # Code generation
├── VM/                  # Virtual Machine source code
│   ├── Types.hs        # Type definitions
│   ├── VM.hs           # VM implementation
│   └── CompilerInterface.hs
├── App/                 # Executable entry points
│   ├── CompilerMain.hs # Compiler main
│   └── VMMain.hs       # VM main
├── examples/            # Example programs
└── tests/               # Test suite
```

## Troubleshooting

### Build Fails with Missing Dependencies

Try updating your package index:
```bash
cabal update
cabal install --only-dependencies
```

### GHC Version Issues

Check your GHC version:
```bash
ghc --version
```

GLaDOS requires GHC 8.10 or later. If you have an older version, upgrade using GHCup:
```bash
ghcup install ghc latest
ghcup set ghc latest
```

### Cabal Cache Issues

If you encounter cache-related errors:
```bash
rm -rf ~/.cabal/store
cabal update
make rebuild
```

### Permission Denied on Executables

Make sure the executables have execute permissions:
```bash
chmod +x glados-compiler glados-vm
```

## Development Setup

For development, you might want to install additional tools:

### Haskell Language Server (HLS)

For IDE support:
```bash
ghcup install hls
```

### HLint

For code quality checks:
```bash
cabal install hlint
```

### Hoogle

For documentation search:
```bash
cabal install hoogle
hoogle generate
```

## Next Steps

Now that you have GLaDOS installed, check out:
- [Quick Start Guide](./quick-start.md)
- [Your First Program](./first-program.md)
- [Language Overview](../language/overview.md)
