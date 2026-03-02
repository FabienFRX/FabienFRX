# GLaDOS — Minimalist LISP Interpreter

GLaDOS is a minimalist LISP interpreter implemented in Haskell. It supports first-class functions, lexical scoping, recursion, conditionals, and basic arithmetic/comparison operators.

```ascii
   ____ _           _           
  / ___| | __ _  __| | ___  ___ 
 | |  _| |/ _` |/ _` |/ _ \/ __|
 | |_| | | (_| | (_| | (_) \__ \
  \____|_|\__,_|\__,_|\___/|___/                                                        
```

## Table of Contents
- [GLaDOS — Minimalist LISP Interpreter (Haskell)](#glados--minimalist-lisp-interpreter-haskell)
  - [Table of Contents](#table-of-contents)
  - [Quick Start](#quick-start)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Running the Interpreter](#running-the-interpreter)
      - [1. Interactive REPL Mode](#1-interactive-repl-mode)
      - [2. File Execution Mode](#2-file-execution-mode)
      - [3. Standard Input Mode](#3-standard-input-mode)
    - [Exit Codes](#exit-codes)
  - [Documentation](#documentation)
    - [Preview the HTML board locally](#preview-the-html-board-locally)
  - [Testing](#testing)
  - [Project Architecture](#project-architecture)
    - [Module Structure](#module-structure)

## Quick Start

Build and start the REPL quickly from the project root:

```bash
make
./glados [filename] or ./glados -i for interactive mode
```

Or build with Stack:

```bash
stack build
./glados [filename] or ./glados -i for interactive mode
```

## Installation

Prerequisites:
- GHC 9.6.6 or compatible
- Stack build tool
- Make

Build from source:

```bash
git clone https://github.com/EpitechPGE3-2025/G-FUN-500-TLS-5-1-glados-4.git
cd G-FUN-500-TLS-5-1-glados-4

# Build using Make
make

# Or build using Stack directly
stack build
```

Common Make targets:

- `make` — Build the project and copy the executable to the repo root
- `make clean` — Clean build artifacts
- `make fclean` — Full clean including binary
- `make re` — Clean and rebuild
- `make test_rule` — Run test suite

## Usage

### Running the Interpreter

GLaDOS can be used in three modes:

#### 1. Interactive REPL Mode
```bash
./glados -i
# or simply
./glados  # (when stdin is a terminal)
```

Example REPL session:
```
GLaDOS LISP Interpreter v1.0
Type expressions to evaluate, or Ctrl+D to exit
> (+ 1 2)
3
> (define x 10)
> (* x 4)
40
> ^D
```

#### 2. File Execution Mode
```bash
./glados program.scm
```

#### 3. Standard Input Mode
```bash
echo "(+ 1 2)" | ./glados
# or
./glados < program.scm
```

### Exit Codes
- `0`: Successful execution
- `84`: Error occurred (parse error, evaluation error, etc.)

## Documentation

- Full documentation (Markdown): `Docs/Documentation.md`
- Original source text: `Docs/Documentation.txt`
- Visual board (HTML): `Docs/index.html`

### Preview the HTML board locally

From the project root, run a simple static server and open the board:

```bash
python3 -m http.server 8000 --directory "$(pwd)"
# Then visit: http://127.0.0.1:8000/Docs/index.html
```

## Testing

```bash
stack test
# or
make test_rule
```

## Project Architecture

### Module Structure
```text
glados/
├── src/
│   ├── Main.hs          # Entry point, REPL, file handling
│   ├── Parser.hs         # S-expression parsing
│   ├── AST.hs           # Data structures and AST conversion
│   └── Evaluator.hs     # Expression evaluation
├── test/
│   └── Spec.hs          # Test suite
├── glados.cabal         # Cabal configuration
├── package.yaml         # Package configuration
├── stack.yaml           # Stack configuration
└── Makefile            # Build automation
```

## License

This project is part of the EPITECH curriculum.

## Authors

- **Fabien Fraixanet** - *Documentation* - [Github](https://github.com/FabienFRX)
- **Andi Berard** - *Unit Tests* - [Github](https://github.com/Andiberard)
- **Gobijan Nathakrishnan** - *Project parsing/AST* - [Github](https://github.com/GobyNathan)
- **Marceau Kersuzan** - *Project error handling* - [Github](https://github.com/PaulTroneysofa)
- **Sofiane Majdoul** - *CI/CD* - [Github](https://github.com/MAJDOUL-Sofiane)
