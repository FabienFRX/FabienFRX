{-
-- EPITECH PROJECT, 2025
-- GLaDOS Part 2
-- File description:
-- Parser Unit Tests
-}

module ParserTests (parserTests) where

import Parser
import AST
import Test.HUnit

-- Test integer parsing
testIntegerParsing :: Test
testIntegerParsing = TestList
    [ TestCase $ assertEqual "Parse positive integer"
        (Right [AInt 42])
        (parseProgram "42")

    , TestCase $ assertEqual "Parse negative integer"
        (Right [AInt (-10)])
        (parseProgram "-10")

    , TestCase $ assertEqual "Parse zero"
        (Right [AInt 0])
        (parseProgram "0")
    ]

-- Test boolean parsing
testBooleanParsing :: Test
testBooleanParsing = TestList
    [ TestCase $ assertEqual "Parse true"
        (Right [ABool True])
        (parseProgram "true")

    , TestCase $ assertEqual "Parse false"
        (Right [ABool False])
        (parseProgram "false")
    ]

-- Test variable parsing
testVariableParsing :: Test
testVariableParsing = TestList
    [ TestCase $ assertEqual "Parse simple variable"
        (Right [AVar "x"])
        (parseProgram "x")

    , TestCase $ assertEqual "Parse variable with numbers"
        (Right [AVar "var123"])
        (parseProgram "var123")
    ]

-- Test assignment parsing
testAssignmentParsing :: Test
testAssignmentParsing = TestList
    [ TestCase $ assertEqual "Parse simple assignment"
        (Right [AAssign "x" (AInt 10)])
        (parseProgram "x = 10")

    , TestCase $ assertEqual "Parse assignment with expression"
        (Right [AAssign "result" (AAdd (AInt 5) (AInt 3))])
        (parseProgram "result = 5 + 3")

    , TestCase $ assertEqual "Parse assignment with semicolon"
        (Right [AAssign "y" (AInt 20)])
        (parseProgram "y = 20;")
    ]

-- Test arithmetic operations
testArithmeticOperations :: Test
testArithmeticOperations = TestList
    [ TestCase $ assertEqual "Parse addition"
        (Right [AAdd (AInt 1) (AInt 2)])
        (parseProgram "1 + 2")

    , TestCase $ assertEqual "Parse subtraction"
        (Right [ASub (AInt 10) (AInt 5)])
        (parseProgram "10 - 5")

    , TestCase $ assertEqual "Parse multiplication"
        (Right [AMul (AInt 3) (AInt 4)])
        (parseProgram "3 * 4")

    , TestCase $ assertEqual "Parse division"
        (Right [ADiv (AInt 20) (AInt 4)])
        (parseProgram "20 / 4")

    , TestCase $ assertEqual "Parse complex expression"
        (Right [AAdd (AMul (AInt 2) (AInt 3)) (AInt 4)])
        (parseProgram "2 * 3 + 4")

    , TestCase $ assertEqual "Parse expression with parentheses"
        (Right [AMul (AInt 2) (AAdd (AInt 3) (AInt 4))])
        (parseProgram "2 * (3 + 4)")
    ]

-- Test comparison operations
testComparisonOperations :: Test
testComparisonOperations = TestList
    [ TestCase $ assertEqual "Parse equality"
        (Right [AEq (AInt 5) (AInt 5)])
        (parseProgram "5 == 5")

    , TestCase $ assertEqual "Parse less than"
        (Right [ALess (AInt 3) (AInt 7)])
        (parseProgram "3 < 7")

    , TestCase $ assertEqual "Parse variable comparison"
        (Right [AEq (AVar "x") (AInt 10)])
        (parseProgram "x == 10")
    ]

-- Test if-else statements
testIfElseStatements :: Test
testIfElseStatements = TestList
    [ TestCase $ assertEqual "Parse if without else"
        (Right [AIf (ABool True) (ASeq [AInt 1]) (ASeq [])])
        (parseProgram "if (true) { 1 }")

    , TestCase $ assertEqual "Parse if with else"
        (Right [AIf (ABool True) (ASeq [AInt 1]) (ASeq [AInt 2])])
        (parseProgram "if (true) { 1 } else { 2 }")

    , TestCase $ assertEqual "Parse if with complex condition"
        (Right [AIf (ALess (AVar "x") (AInt 10))
                    (ASeq [AAssign "y" (AInt 1)])
                    (ASeq [AAssign "y" (AInt 0)])])
        (parseProgram "if (x < 10) { y = 1 } else { y = 0 }")
    ]

-- Test function definition
testFunctionDefinition :: Test
testFunctionDefinition = TestList
    [ TestCase $ assertEqual "Parse simple function"
        (Right [AFuncDef "add" ["a", "b"] (ASeq [AReturn (AAdd (AVar "a") (AVar "b"))])])
        (parseProgram "fn add(a, b) { return a + b }")

    , TestCase $ assertEqual "Parse function with no parameters"
        (Right [AFuncDef "getZero" [] (ASeq [AReturn (AInt 0)])])
        (parseProgram "fn getZero() { return 0 }")

    , TestCase $ assertEqual "Parse function with multiple statements"
        (Right [AFuncDef "calculate" ["x"]
                (ASeq [AAssign "y" (AMul (AVar "x") (AInt 2)),
                       AReturn (AVar "y")])])
        (parseProgram "fn calculate(x) { y = x * 2 return y }")
    ]

-- Test function calls
testFunctionCalls :: Test
testFunctionCalls = TestList
    [ TestCase $ assertEqual "Parse function call with no arguments"
        (Right [ACall "foo" []])
        (parseProgram "foo()")

    , TestCase $ assertEqual "Parse function call with one argument"
        (Right [ACall "square" [AInt 5]])
        (parseProgram "square(5)")

    , TestCase $ assertEqual "Parse function call with multiple arguments"
        (Right [ACall "add" [AInt 3, AInt 7]])
        (parseProgram "add(3, 7)")

    , TestCase $ assertEqual "Parse nested function calls"
        (Right [ACall "outer" [ACall "inner" [AInt 5]]])
        (parseProgram "outer(inner(5))")
    ]

-- Test return statements
testReturnStatements :: Test
testReturnStatements = TestList
    [ TestCase $ assertEqual "Parse return with integer"
        (Right [AReturn (AInt 42)])
        (parseProgram "return 42")

    , TestCase $ assertEqual "Parse return with expression"
        (Right [AReturn (AAdd (AInt 1) (AInt 2))])
        (parseProgram "return 1 + 2")

    , TestCase $ assertEqual "Parse return with semicolon"
        (Right [AReturn (AVar "x")])
        (parseProgram "return x;")
    ]

-- Test multiple statements
testMultipleStatements :: Test
testMultipleStatements = TestList
    [ TestCase $ assertEqual "Parse two assignments"
        (Right [AAssign "x" (AInt 10), AAssign "y" (AInt 20)])
        (parseProgram "x = 10 y = 20")

    , TestCase $ assertEqual "Parse mixed statements"
        (Right [AAssign "a" (AInt 5),
                AAssign "b" (AInt 3),
                AAdd (AVar "a") (AVar "b")])
        (parseProgram "a = 5 b = 3 a + b")
    ]

-- Test whitespace handling
testWhitespaceHandling :: Test
testWhitespaceHandling = TestList
    [ TestCase $ assertEqual "Parse with extra spaces"
        (Right [AAdd (AInt 1) (AInt 2)])
        (parseProgram "  1   +   2  ")

    , TestCase $ assertEqual "Parse with newlines"
        (Right [AAssign "x" (AInt 10)])
        (parseProgram "\nx = 10\n")
    ]

-- Test error cases
testErrorCases :: Test
testErrorCases = TestList
    [ TestCase $ assertBool "Empty input should fail gracefully"
        (case parseProgram "" of
            Left _ -> True
            Right [] -> True
            _ -> False)

    , TestCase $ assertBool "Invalid syntax should fail"
        (case parseProgram "1 + + 2" of
            Left _ -> True
            Right _ -> False)
    ]

-- Collect all parser tests
parserTests :: Test
parserTests = TestLabel "Parser Tests" $ TestList
    [ TestLabel "Integer Parsing" testIntegerParsing
    , TestLabel "Boolean Parsing" testBooleanParsing
    , TestLabel "Variable Parsing" testVariableParsing
    , TestLabel "Assignment Parsing" testAssignmentParsing
    , TestLabel "Arithmetic Operations" testArithmeticOperations
    , TestLabel "Comparison Operations" testComparisonOperations
    , TestLabel "If-Else Statements" testIfElseStatements
    , TestLabel "Function Definition" testFunctionDefinition
    , TestLabel "Function Calls" testFunctionCalls
    , TestLabel "Return Statements" testReturnStatements
    , TestLabel "Multiple Statements" testMultipleStatements
    , TestLabel "Whitespace Handling" testWhitespaceHandling
    , TestLabel "Error Cases" testErrorCases
    ]
