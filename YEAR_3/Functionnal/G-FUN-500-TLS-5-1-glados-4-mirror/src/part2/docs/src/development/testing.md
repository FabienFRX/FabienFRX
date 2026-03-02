# Testing

GLaDOS includes a comprehensive test suite to ensure correctness.

## Running Tests

### Quick Test

```bash
cd src/part2/Build
make test
```

### Manual Test Run

```bash
cabal test
```

## Test Structure

Tests are located in `src/part2/tests/`:

```
tests/
├── TestMain.hs           # Test suite entry point
├── ParserTests.hs        # Parser tests
├── CompilerTests.hs      # Compiler tests
├── VMTests.hs            # VM tests
└── IntegrationTests.hs   # End-to-end tests
```

## Test Framework

GLaDOS uses HSpec for testing:

```haskell
import Test.Hspec

spec :: Spec
spec = do
    describe "Parser" $ do
        it "parses integers" $ do
            parseExpr "42" `shouldBe` Right (AInt 42)
```

## Test Categories

### Parser Tests

Test the parser with various inputs:

```haskell
describe "Parser" $ do
    it "parses addition" $ do
        parseExpr "10 + 20" `shouldBe` Right (AAdd (AInt 10) (AInt 20))

    it "parses function definitions" $ do
        parseProgram "fn main() { return 42 }" `shouldSatisfy` isRight
```

### Compiler Tests

Test code generation:

```haskell
describe "Compiler" $ do
    it "compiles integer literals" $ do
        compileAST (AInt 42) `shouldBe` Right [Push (VInt 42)]

    it "compiles binary operations" $ do
        let ast = AAdd (AInt 10) (AInt 20)
        compileAST ast `shouldBe` Right [Push (VInt 10), Push (VInt 20), BinOp Add]
```

### VM Tests

Test VM execution:

```haskell
describe "VM" $ do
    it "executes push and pop" $ do
        let instructions = [Push (VInt 42), Return]
        runVM instructions initVM `shouldSatisfy` isRight

    it "executes binary operations" $ do
        let instructions = [Push (VInt 10), Push (VInt 20), BinOp Add, Return]
        result <- runProgram instructions Map.empty
        result `shouldBe` Right (VInt 30)
```

### Integration Tests

End-to-end tests:

```haskell
describe "Integration" $ do
    it "compiles and runs factorial" $ do
        let source = "fn factorial(n) { ... } fn main() { return factorial(5) }"
        case parseProgram source of
            Right ast -> do
                case compile ast of
                    Right (entry, funcs) -> do
                        result <- runProgram entry funcs
                        result `shouldBe` Right (VInt 120)
```

## Running Specific Tests

### Run Parser Tests Only

```bash
cabal test --test-options="--match Parser"
```

### Run with Verbose Output

```bash
cabal test --test-show-details=streaming
```

## Test Coverage

Check test coverage:

```bash
cabal test --enable-coverage
```

View coverage report:

```bash
firefox dist-newstyle/build/.../hpc/html/glados-test/hpc_index.html
```

## Writing Tests

### Test Template

```haskell
module MyTests (spec) where

import Test.Hspec

spec :: Spec
spec = do
    describe "Feature" $ do
        it "does something" $ do
            result <- computeSomething
            result `shouldBe` expectedValue
```

### Assertions

Common HSpec matchers:

```haskell
x `shouldBe` y              -- Equality
x `shouldSatisfy` predicate -- Predicate test
x `shouldReturn` y          -- IO result
shouldThrow                 -- Exception test
```

## Example Programs Testing

Test with example programs:

```bash
cd Build

for file in ../examples/*.gld; do
    echo "Testing: $file"
    ./glados-compiler -o test.bc "$file"
    ./glados-vm test.bc
    echo "---"
done
```

## Continuous Integration

For CI/CD:

```yaml
# .github/workflows/test.yml
name: Test
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: haskell/actions/setup@v1
      - run: cabal test
```

## Debugging Failed Tests

### Verbose Test Output

```bash
cabal test --test-show-details=direct
```

### Run Single Test

```bash
cabal test --test-options="--match 'specific test name'"
```

### Enable Debug Logging

Add debug prints in tests:

```haskell
it "test with debugging" $ do
    let result = computeSomething
    putStrLn $ "Debug: " ++ show result
    result `shouldBe` expected
```

## Performance Testing

Benchmark recursive functions:

```bash
# Time factorial(20)
time ./glados-vm factorial.bc
```

## Property-Based Testing

Using QuickCheck for property tests:

```haskell
import Test.QuickCheck

prop_addition_commutative :: Int -> Int -> Bool
prop_addition_commutative a b =
    compileAndRun (a + b) == compileAndRun (b + a)
```

## Next Steps

- Read [contributing guidelines](./contributing.md)
- Learn about [building](./building.md)
