{-
-- EPITECH PROJECT, 2025
-- GLaDOS Part 2
-- File description:
-- VM Unit Tests
-}

module VMTests (vmTests) where

import VM
import Types
import Test.HUnit
import qualified Data.Map as Map

-- Test stack operations
testStackOperations :: Test
testStackOperations = TestList
    [ TestCase $ assertEqual "Push integer on stack"
        (Right (VInt 42))
        (runProgram [Push (VInt 42)] Map.empty)

    , TestCase $ assertEqual "Push boolean on stack"
        (Right (VBool True))
        (runProgram [Push (VBool True)] Map.empty)

    , TestCase $ assertEqual "Multiple pushes return top value"
        (Right (VInt 2))
        (runProgram [Push (VInt 1), Push (VInt 2)] Map.empty)
    ]

-- Test arithmetic operations
testArithmeticOperations :: Test
testArithmeticOperations = TestList
    [ TestCase $ assertEqual "Addition: 5 + 3 = 8"
        (Right (VInt 8))
        (runProgram [Push (VInt 5), Push (VInt 3), BinOp Add] Map.empty)

    , TestCase $ assertEqual "Subtraction: 10 - 4 = 6"
        (Right (VInt 6))
        (runProgram [Push (VInt 10), Push (VInt 4), BinOp Sub] Map.empty)

    , TestCase $ assertEqual "Multiplication: 6 * 7 = 42"
        (Right (VInt 42))
        (runProgram [Push (VInt 6), Push (VInt 7), BinOp Mul] Map.empty)

    , TestCase $ assertEqual "Division: 20 / 5 = 4"
        (Right (VInt 4))
        (runProgram [Push (VInt 20), Push (VInt 5), BinOp Div] Map.empty)

    , TestCase $ assertEqual "Complex expression: (2 + 3) * 4 = 20"
        (Right (VInt 20))
        (runProgram [Push (VInt 2), Push (VInt 3), BinOp Add,
                     Push (VInt 4), BinOp Mul] Map.empty)
    ]

-- Test comparison operations
testComparisonOperations :: Test
testComparisonOperations = TestList
    [ TestCase $ assertEqual "Equality: 5 == 5 = true"
        (Right (VBool True))
        (runProgram [Push (VInt 5), Push (VInt 5), BinOp Eq] Map.empty)

    , TestCase $ assertEqual "Equality: 5 == 3 = false"
        (Right (VBool False))
        (runProgram [Push (VInt 5), Push (VInt 3), BinOp Eq] Map.empty)

    , TestCase $ assertEqual "Less than: 3 < 7 = true"
        (Right (VBool True))
        (runProgram [Push (VInt 3), Push (VInt 7), BinOp Less] Map.empty)

    , TestCase $ assertEqual "Less than: 10 < 5 = false"
        (Right (VBool False))
        (runProgram [Push (VInt 10), Push (VInt 5), BinOp Less] Map.empty)

    , TestCase $ assertEqual "Boolean equality: true == true"
        (Right (VBool True))
        (runProgram [Push (VBool True), Push (VBool True), BinOp Eq] Map.empty)

    , TestCase $ assertEqual "Boolean equality: true == false"
        (Right (VBool False))
        (runProgram [Push (VBool True), Push (VBool False), BinOp Eq] Map.empty)
    ]

-- Test variable operations
testVariableOperations :: Test
testVariableOperations = TestList
    [ TestCase $ assertEqual "Store and load variable"
        (Right (VInt 42))
        (runProgram [Push (VInt 42), StoreVar "x", LoadVar "x"] Map.empty)

    , TestCase $ assertEqual "Store multiple variables"
        (Right (VInt 20))
        (runProgram [Push (VInt 10), StoreVar "x",
                     Push (VInt 20), StoreVar "y",
                     LoadVar "y"] Map.empty)

    , TestCase $ assertEqual "Update variable value"
        (Right (VInt 100))
        (runProgram [Push (VInt 10), StoreVar "x",
                     Push (VInt 100), StoreVar "x",
                     LoadVar "x"] Map.empty)

    , TestCase $ case runProgram [LoadVar "undefined"] Map.empty of
        Left _ -> assertBool "Should fail when loading undefined variable" True
        Right _ -> assertFailure "Should have failed"
    ]

-- Test conditional jumps
testConditionalJumps :: Test
testConditionalJumps = TestList
    [ TestCase $ assertEqual "Jump when false (VBool False)"
        (Right (VInt 99))
        (runProgram [Push (VBool False),
                     JumpIfFalse 1,
                     Push (VInt 1),
                     Push (VInt 99)] Map.empty)

    , TestCase $ assertEqual "Don't jump when true (VBool True)"
        (Right (VInt 99))
        (runProgram [Push (VBool True),
                     JumpIfFalse 1,
                     Push (VInt 1),
                     Push (VInt 99)] Map.empty)

    , TestCase $ assertEqual "Jump when zero (VInt 0)"
        (Right (VInt 99))
        (runProgram [Push (VInt 0),
                     JumpIfFalse 1,
                     Push (VInt 1),
                     Push (VInt 99)] Map.empty)

    , TestCase $ assertEqual "Don't jump when non-zero (VInt 5)"
        (Right (VInt 99))
        (runProgram [Push (VInt 5),
                     JumpIfFalse 1,
                     Push (VInt 1),
                     Push (VInt 99)] Map.empty)
    ]

-- Test unconditional jumps
testUnconditionalJumps :: Test
testUnconditionalJumps = TestList
    [ TestCase $ assertEqual "Unconditional jump forward"
        (Right (VInt 99))
        (runProgram [Jump 1,
                     Push (VInt 1),
                     Push (VInt 99)] Map.empty)

    , TestCase $ assertEqual "Jump over multiple instructions"
        (Right (VInt 42))
        (runProgram [Jump 3,
                     Push (VInt 1),
                     Push (VInt 2),
                     Push (VInt 3),
                     Push (VInt 42)] Map.empty)
    ]

-- Test function calls
testFunctionCalls :: Test
testFunctionCalls = TestList
    [ TestCase $
        let funcs = Map.fromList [("getFortyTwo", ([], [Push (VInt 42), Return]))]
            result = runProgram [CallFunc "getFortyTwo" 0] funcs
        in assertEqual "Call function with no parameters"
            (Right (VInt 42))
            result

    , TestCase $
        let funcs = Map.fromList [("double", (["x"], [LoadVar "x", LoadVar "x", BinOp Add, Return]))]
            result = runProgram [Push (VInt 21), CallFunc "double" 1] funcs
        in assertEqual "Call function with one parameter"
            (Right (VInt 42))
            result

    , TestCase $
        let funcs = Map.fromList [("add", (["a", "b"], [LoadVar "a", LoadVar "b", BinOp Add, Return]))]
            result = runProgram [Push (VInt 10), Push (VInt 32), CallFunc "add" 2] funcs
        in assertEqual "Call function with two parameters"
            (Right (VInt 42))
            result

    , TestCase $ case runProgram [CallFunc "undefined" 0] Map.empty of
        Left _ -> assertBool "Should fail when calling undefined function" True
        Right _ -> assertFailure "Should have failed"
    ]

-- Test return statements
testReturnStatements :: Test
testReturnStatements = TestList
    [ TestCase $
        let funcs = Map.fromList [("earlyReturn", ([],
                [Push (VInt 10),
                 Return,
                 Push (VInt 20)]))]
            result = runProgram [CallFunc "earlyReturn" 0] funcs
        in assertEqual "Early return stops execution"
            (Right (VInt 10))
            result
    ]

-- Test error handling
testErrorHandling :: Test
testErrorHandling = TestList
    [ TestCase $ case runProgram [Push (VInt 10), Push (VInt 0), BinOp Div] Map.empty of
        Left _ -> assertBool "Division by zero should fail" True
        Right _ -> assertFailure "Should have failed on division by zero"

    , TestCase $ case runProgram [BinOp Add] Map.empty of
        Left _ -> assertBool "Binary op with empty stack should fail" True
        Right _ -> assertFailure "Should have failed"

    , TestCase $ case runProgram [Push (VInt 1), BinOp Add] Map.empty of
        Left _ -> assertBool "Binary op with one operand should fail" True
        Right _ -> assertFailure "Should have failed"

    , TestCase $ case runProgram [StoreVar "x"] Map.empty of
        Left _ -> assertBool "Store with empty stack should fail" True
        Right _ -> assertFailure "Should have failed"
    ]

-- Test complex programs
testComplexPrograms :: Test
testComplexPrograms = TestList
    [ TestCase $ assertEqual "If-then-else: if (true) 1 else 2"
        (Right (VInt 1))
        (runProgram [Push (VBool True),
                     JumpIfFalse 2,
                     Push (VInt 1),
                     Jump 1,
                     Push (VInt 2)] Map.empty)

    , TestCase $ assertEqual "If-then-else: if (false) 1 else 2"
        (Right (VInt 2))
        (runProgram [Push (VBool False),
                     JumpIfFalse 2,
                     Push (VInt 1),
                     Jump 1,
                     Push (VInt 2)] Map.empty)

    , TestCase $
        let funcs = Map.fromList [("factorial3", ([],
                [Push (VInt 3),
                 Push (VInt 2), BinOp Mul,
                 Push (VInt 1), BinOp Mul,
                 Return]))]
            result = runProgram [CallFunc "factorial3" 0] funcs
        in assertEqual "Factorial of 3 = 6"
            (Right (VInt 6))
            result
    ]

-- Test VM state management
testVMStateManagement :: Test
testVMStateManagement = TestList
    [ TestCase $
        let instructions = [Push (VInt 10), StoreVar "x", LoadVar "x"]
            result = runVM instructions initVM
        in case result of
            Right state -> do
                assertEqual "Stack should have value" [VInt 10] (stack state)
                case Map.lookup "x" (vars state) of
                    Just (VInt 10) -> return ()
                    _ -> assertFailure "Variable x should be 10"
            Left err -> assertFailure $ "VM execution failed: " ++ err
    ]

-- Collect all VM tests
vmTests :: Test
vmTests = TestLabel "VM Tests" $ TestList
    [ TestLabel "Stack Operations" testStackOperations
    , TestLabel "Arithmetic Operations" testArithmeticOperations
    , TestLabel "Comparison Operations" testComparisonOperations
    , TestLabel "Variable Operations" testVariableOperations
    , TestLabel "Conditional Jumps" testConditionalJumps
    , TestLabel "Unconditional Jumps" testUnconditionalJumps
    , TestLabel "Function Calls" testFunctionCalls
    , TestLabel "Return Statements" testReturnStatements
    , TestLabel "Error Handling" testErrorHandling
    , TestLabel "Complex Programs" testComplexPrograms
    , TestLabel "VM State Management" testVMStateManagement
    ]
