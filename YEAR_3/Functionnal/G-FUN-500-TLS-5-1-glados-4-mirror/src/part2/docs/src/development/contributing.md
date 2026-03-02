# Contributing

Thank you for your interest in contributing to GLaDOS!

## Getting Started

1. Fork the repository
2. Clone your fork
3. Create a feature branch
4. Make your changes
5. Run tests
6. Submit a pull request

## Development Setup

### Prerequisites

- GHC 8.10+
- Cabal 3.0+
- Git
- Text editor or IDE with Haskell support

### Clone Repository

```bash
git clone https://github.com/yourusername/glados.git
cd glados/src/part2
```

### Build and Test

```bash
cd Build
make
make test
```

## Code Style

### Haskell Style

Follow standard Haskell conventions:

```haskell
-- Good: descriptive names, proper spacing
compileExpression :: AST -> Either String [Instruction]
compileExpression (AInt n) = Right [Push (VInt n)]

-- Bad: unclear names, poor formatting
ce::AST->Either String[Instruction]
ce(AInt n)=Right[Push$VInt n]
```

### Formatting

- **Indentation**: 4 spaces
- **Line length**: Max 100 characters
- **Comments**: Explain why, not what

### Module Structure

```haskell
{-
-- EPITECH PROJECT, 2025
-- GLaDOS Part 2
-- File description:
-- Module purpose
-}

module ModuleName (exportedFunctions) where

import qualified Data.Map as Map

-- Implementation
```

## Adding Features

### New Language Features

1. Update AST in `Compiler/AST.hs`
2. Add parser rules in `Compiler/Parser.hs`
3. Implement code generation in `Compiler/Compiler.hs`
4. Add VM instruction if needed in `VM/Types.hs`
5. Implement execution in `VM/VM.hs`
6. Add tests
7. Update documentation

### New Instructions

1. Add to `Instruction` type in `VM/Types.hs`
2. Implement execution in `VM/VM.hs`
3. Add Binary instance for serialization
4. Update compiler to generate instruction
5. Add tests
6. Document in architecture docs

## Testing

All contributions must include tests:

```haskell
-- tests/MyTests.hs
module MyTests (spec) where

import Test.Hspec

spec :: Spec
spec = do
    describe "My Feature" $ do
        it "works correctly" $ do
            result <- myFunction input
            result `shouldBe` expected
```

### Test Coverage

Aim for high test coverage:

```bash
cabal test --enable-coverage
```

## Documentation

Update documentation when adding features:

1. **Code comments**: Explain complex logic
2. **Function docs**: Document exported functions
3. **README**: Update if user-facing changes
4. **mdbook docs**: Add examples and explanations

### Documentation Example

```haskell
-- | Compiles an AST node to bytecode instructions.
-- Returns either an error message or a list of instructions.
--
-- Example:
-- >>> compileAST (AInt 42)
-- Right [Push (VInt 42)]
compileAST :: AST -> Either String [Instruction]
```

## Pull Request Process

### Before Submitting

- [ ] Code follows style guide
- [ ] Tests pass (`make test`)
- [ ] New tests added for new features
- [ ] Documentation updated
- [ ] Commit messages are clear

### Commit Messages

Use clear, descriptive commit messages:

```
Add support for modulo operator

- Add AMod to AST
- Implement parser rule for %
- Add Mod to BinOp types
- Generate modulo bytecode
- Add VM execution for modulo
- Add tests for modulo operations
```

### PR Template

```markdown
## Description
Brief description of changes

## Changes
- List of changes
- One per line

## Testing
How to test these changes

## Documentation
Updated documentation (yes/no)
```

## Code Review

All PRs require review:

- Follow feedback constructively
- Update based on comments
- Keep discussion professional
- Be patient

## Issue Reporting

When reporting bugs:

1. **Clear title**: Describe the issue
2. **Steps to reproduce**: How to trigger the bug
3. **Expected behavior**: What should happen
4. **Actual behavior**: What actually happens
5. **Environment**: OS, GHC version, etc.

### Issue Template

```markdown
## Bug Description
Clear description of the bug

## Steps to Reproduce
1. Step one
2. Step two
3. See error

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Environment
- OS: Linux/macOS/Windows
- GHC: 8.10.7
- Cabal: 3.4.0.0
```

## Feature Requests

When suggesting features:

1. **Use case**: Why is this needed?
2. **Proposed solution**: How should it work?
3. **Alternatives**: Other approaches considered
4. **Examples**: Show how it would be used

## Community Guidelines

- Be respectful and inclusive
- Help others learn
- Give constructive feedback
- Follow code of conduct

## License

By contributing, you agree that your contributions will be licensed under the BSD-3-Clause license.

## Getting Help

- Open an issue for questions
- Check existing documentation
- Ask in discussions

## Thank You!

Your contributions make GLaDOS better for everyone!
