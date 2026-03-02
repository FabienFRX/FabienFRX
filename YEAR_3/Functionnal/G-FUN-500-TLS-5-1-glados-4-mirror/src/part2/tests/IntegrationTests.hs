{-
-- EPITECH PROJECT, 2025
-- GLaDOS Part 2
-- File description:
-- Integration Tests (End-to-End)
-}

module IntegrationTests (integrationTests) where

import Parser
import Compiler
import VM
import AST
import Test.HUnit

-- Helper function to run the entire pipeline
-- Wraps simple expressions in main() if needed
runFullPipeline :: String -> Either String String
runFullPipeline source = do
    ast <- parseProgram source
    let wrappedAST = wrapInMain ast
    (mainCode, funcs) <- compile wrappedAST
    result <- runProgram mainCode funcs
    return $ show result

-- Wrap simple expressions in a main() function
wrapInMain :: [AST] -> [AST]
wrapInMain asts
    | hasMain asts = asts  -- Already has main(), don't modify
    | hasFunctions asts = addMainToFunctions asts  -- Has functions but no main()
    | null asts = [AFuncDef "main" [] (ASeq [AReturn (AInt 0)])]
    | otherwise = [AFuncDef "main" [] (makeBody asts)]
  where
    hasMain [] = False
    hasMain (AFuncDef "main" _ _ : _) = True
    hasMain (_ : rest) = hasMain rest

    hasFunctions = any isFuncDef
    isFuncDef (AFuncDef _ _ _) = True
    isFuncDef _ = False

    -- Add main() that executes non-function statements and returns last expression
    addMainToFunctions nodes =
        let (funcs, nonFuncs) = partition isFuncDef nodes
        in if null nonFuncs
           then funcs ++ [AFuncDef "main" [] (ASeq [AReturn (AInt 0)])]  -- No code, return 0
           else funcs ++ [AFuncDef "main" [] (makeBody nonFuncs)]

    partition :: (a -> Bool) -> [a] -> ([a], [a])
    partition _ [] = ([], [])
    partition p (x:xs)
        | p x = let (ys, zs) = partition p xs in (x:ys, zs)
        | otherwise = let (ys, zs) = partition p xs in (ys, x:zs)

    makeBody [single] = wrapSingle single
    makeBody multiple =
        let allButLast = init multiple
            lastExpr = last multiple
        in ASeq (allButLast ++ [wrapSingle lastExpr])

    wrapSingle (AReturn e) = AReturn e
    wrapSingle expr = AReturn expr

-- Test simple expressions
testSimpleExpressions :: Test
testSimpleExpressions = TestList
    [ TestCase $ assertEqual "Integer literal: 42"
        (Right "VInt 42")
        (runFullPipeline "42")

    , TestCase $ assertEqual "Boolean literal: true"
        (Right "VBool True")
        (runFullPipeline "true")

    , TestCase $ assertEqual "Boolean literal: false"
        (Right "VBool False")
        (runFullPipeline "false")
    ]

-- Test arithmetic expressions
testArithmeticExpressions :: Test
testArithmeticExpressions = TestList
    [ TestCase $ assertEqual "Addition: 5 + 3"
        (Right "VInt 8")
        (runFullPipeline "5 + 3")

    , TestCase $ assertEqual "Subtraction: 10 - 4"
        (Right "VInt 6")
        (runFullPipeline "10 - 4")

    , TestCase $ assertEqual "Multiplication: 6 * 7"
        (Right "VInt 42")
        (runFullPipeline "6 * 7")

    , TestCase $ assertEqual "Division: 20 / 5"
        (Right "VInt 4")
        (runFullPipeline "20 / 5")

    , TestCase $ assertEqual "Complex expression: 2 * 3 + 4"
        (Right "VInt 10")
        (runFullPipeline "2 * 3 + 4")

    , TestCase $ assertEqual "Expression with parentheses: 2 * (3 + 4)"
        (Right "VInt 14")
        (runFullPipeline "2 * (3 + 4)")

    , TestCase $ assertEqual "Nested operations: (10 + 5) * (8 - 3)"
        (Right "VInt 75")
        (runFullPipeline "(10 + 5) * (8 - 3)")
    ]

-- Test comparison expressions
testComparisonExpressions :: Test
testComparisonExpressions = TestList
    [ TestCase $ assertEqual "Equality: 5 == 5"
        (Right "VBool True")
        (runFullPipeline "5 == 5")

    , TestCase $ assertEqual "Equality: 5 == 3"
        (Right "VBool False")
        (runFullPipeline "5 == 3")

    , TestCase $ assertEqual "Less than: 3 < 7"
        (Right "VBool True")
        (runFullPipeline "3 < 7")

    , TestCase $ assertEqual "Less than: 10 < 5"
        (Right "VBool False")
        (runFullPipeline "10 < 5")

    , TestCase $ assertEqual "Complex comparison: (2 + 3) == 5"
        (Right "VBool True")
        (runFullPipeline "(2 + 3) == 5")
    ]

-- Test variables
testVariables :: Test
testVariables = TestList
    [ TestCase $ assertEqual "Simple assignment and use"
        (Right "VInt 42")
        (runFullPipeline "x = 42 x")

    , TestCase $ assertEqual "Multiple assignments"
        (Right "VInt 30")
        (runFullPipeline "x = 10 y = 20 x + y")

    , TestCase $ assertEqual "Variable reassignment"
        (Right "VInt 100")
        (runFullPipeline "x = 10 x = 100 x")

    , TestCase $ assertEqual "Assignment with expression"
        (Right "VInt 15")
        (runFullPipeline "x = 5 + 10 x")

    , TestCase $ assertEqual "Using variable in expression"
        (Right "VInt 20")
        (runFullPipeline "x = 10 y = x * 2 y")
    ]

-- Test if-else statements
testIfElseStatements :: Test
testIfElseStatements = TestList
    [ TestCase $ assertEqual "If-true returns then branch"
        (Right "VInt 1")
        (runFullPipeline "if (true) { 1 } else { 2 }")

    , TestCase $ assertEqual "If-false returns else branch"
        (Right "VInt 2")
        (runFullPipeline "if (false) { 1 } else { 2 }")

    , TestCase $ assertEqual "If with comparison"
        (Right "VInt 10")
        (runFullPipeline "x = 5 if (x < 10) { 10 } else { 20 }")

    , TestCase $ assertEqual "If with assignment in branches"
        (Right "VInt 100")
        (runFullPipeline "x = 5 if (x < 10) { y = 100 } else { y = 200 } y")

    , TestCase $ assertEqual "Nested if statements"
        (Right "VInt 3")
        (runFullPipeline "x = 5 if (x < 10) { if (x < 7) { 3 } else { 4 } } else { 5 }")
    ]

-- Test function definition and calls
testFunctions :: Test
testFunctions = TestList
    [ TestCase $ assertEqual "Simple function call"
        (Right "VInt 42")
        (runFullPipeline "fn getFortyTwo() { return 42 } getFortyTwo()")

    , TestCase $ assertEqual "Function with one parameter"
        (Right "VInt 10")
        (runFullPipeline "fn double(x) { return x * 2 } double(5)")

    , TestCase $ assertEqual "Function with two parameters"
        (Right "VInt 8")
        (runFullPipeline "fn add(a, b) { return a + b } add(3, 5)")

    , TestCase $ assertEqual "Function with computation"
        (Right "VInt 13")
        (runFullPipeline "fn sum3(a, b, c) { return a + b + c } sum3(4, 5, 4)")

    , TestCase $ assertEqual "Multiple function calls"
        (Right "VInt 14")
        (runFullPipeline "fn double(x) { return x * 2 } fn add(a, b) { return a + b } add(double(3), double(4))")
    ]

-- Test complex programs
testComplexPrograms :: Test
testComplexPrograms = TestList
    [ TestCase $ assertEqual "Factorial-like computation"
        (Right "VInt 6")
        (runFullPipeline "fn factorial3() { return 3 * 2 * 1 } factorial3()")

    , TestCase $ assertEqual "Max function simulation"
        (Right "VInt 10")
        (runFullPipeline "fn max(a, b) { if (a < b) { return b } else { return a } } max(5, 10)")

    , TestCase $ assertEqual "Absolute value simulation"
        (Right "VInt 5")
        (runFullPipeline "fn abs(x) { if (x < 0) { return 0 - x } else { return x } } abs(5)")

    , TestCase $ assertEqual "Complex calculation with variables and functions"
        (Right "VInt 42")
        (runFullPipeline "fn add(a, b) { return a + b } x = 10 y = 20 z = add(x, y) w = z + 12 w")

    , TestCase $ assertEqual "Function using local computation"
        (Right "VInt 25")
        (runFullPipeline "fn square(x) { temp = x * x return temp } square(5)")
    ]

-- Test edge cases
testEdgeCases :: Test
testEdgeCases = TestList
    [ TestCase $ assertEqual "Negative numbers"
        (Right "VInt (-5)")
        (runFullPipeline "-5")

    , TestCase $ assertEqual "Zero"
        (Right "VInt 0")
        (runFullPipeline "0")

    , TestCase $ assertEqual "Subtraction resulting in negative"
        (Right "VInt (-3)")
        (runFullPipeline "5 - 8")

    , TestCase $ assertEqual "Division exact"
        (Right "VInt 4")
        (runFullPipeline "12 / 3")

    , TestCase $ assertEqual "Boolean operations"
        (Right "VBool True")
        (runFullPipeline "true == true")

    , TestCase $ assertEqual "Mixed boolean operations"
        (Right "VBool False")
        (runFullPipeline "true == false")
    ]

-- Test error cases
testErrorCases :: Test
testErrorCases = TestList
    [ TestCase $ case runFullPipeline "x" of
        Left _ -> return ()
        Right _ -> assertFailure "Should fail on undefined variable"

    , TestCase $ case runFullPipeline "10 / 0" of
        Left _ -> return ()
        Right _ -> assertFailure "Should fail on division by zero"

    , TestCase $ case runFullPipeline "foo()" of
        Left _ -> return ()
        Right _ -> assertFailure "Should fail on undefined function"

    , TestCase $ case runFullPipeline "fn add(a, b) { return a + b } add(1)" of
        Left _ -> return ()
        Right _ -> assertFailure "Should fail on wrong number of arguments"
    ]

-- Test sequential execution
testSequentialExecution :: Test
testSequentialExecution = TestList
    [ TestCase $ assertEqual "Multiple statements execute in order"
        (Right "VInt 30")
        (runFullPipeline "x = 10 y = 20 x + y")

    , TestCase $ assertEqual "Reassignment works correctly"
        (Right "VInt 15")
        (runFullPipeline "x = 5 x = 10 x = 15 x")

    , TestCase $ assertEqual "Function and variable interaction"
        (Right "VInt 20")
        (runFullPipeline "fn double(x) { return x * 2 } a = 5 b = double(a) b + b")
    ]

-- Collect all integration tests
integrationTests :: Test
integrationTests = TestLabel "Integration Tests (End-to-End)" $ TestList
    [ TestLabel "Simple Expressions" testSimpleExpressions
    , TestLabel "Arithmetic Expressions" testArithmeticExpressions
    , TestLabel "Comparison Expressions" testComparisonExpressions
    , TestLabel "Variables" testVariables
    , TestLabel "If-Else Statements" testIfElseStatements
    , TestLabel "Functions" testFunctions
    , TestLabel "Complex Programs" testComplexPrograms
    , TestLabel "Edge Cases" testEdgeCases
    , TestLabel "Error Cases" testErrorCases
    , TestLabel "Sequential Execution" testSequentialExecution
    ]
