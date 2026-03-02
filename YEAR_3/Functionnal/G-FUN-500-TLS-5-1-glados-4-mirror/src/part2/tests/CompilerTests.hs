{-
-- EPITECH PROJECT, 2025
-- GLaDOS Part 2
-- File description:
-- Compiler Unit Tests
-}

module CompilerTests (compilerTests) where

import Compiler
import AST
import Types
import Test.HUnit
import qualified Data.Map as Map

-- Helper function to check if compilation succeeds
isRight :: Either a b -> Bool
isRight (Right _) = True
isRight _ = False

-- Test literal compilation
testLiteralCompilation :: Test
testLiteralCompilation = TestList
    [ TestCase $ assertEqual "Compile integer literal"
        (Right [Push (VInt 42)])
        (compileAST (AInt 42))

    , TestCase $ assertEqual "Compile boolean true"
        (Right [Push (VBool True)])
        (compileAST (ABool True))

    , TestCase $ assertEqual "Compile boolean false"
        (Right [Push (VBool False)])
        (compileAST (ABool False))
    ]

-- Test variable compilation
testVariableCompilation :: Test
testVariableCompilation = TestList
    [ TestCase $ assertEqual "Compile variable load"
        (Right [LoadVar "x"])
        (compileAST (AVar "x"))

    , TestCase $ assertEqual "Compile variable assignment"
        (Right [Push (VInt 10), StoreVar "x"])
        (compileAST (AAssign "x" (AInt 10)))

    , TestCase $ assertEqual "Compile assignment with expression"
        (Right [Push (VInt 5), Push (VInt 3), BinOp Add, StoreVar "result"])
        (compileAST (AAssign "result" (AAdd (AInt 5) (AInt 3))))
    ]

-- Test arithmetic operations compilation
testArithmeticCompilation :: Test
testArithmeticCompilation = TestList
    [ TestCase $ assertEqual "Compile addition"
        (Right [Push (VInt 1), Push (VInt 2), BinOp Add])
        (compileAST (AAdd (AInt 1) (AInt 2)))

    , TestCase $ assertEqual "Compile subtraction"
        (Right [Push (VInt 10), Push (VInt 5), BinOp Sub])
        (compileAST (ASub (AInt 10) (AInt 5)))

    , TestCase $ assertEqual "Compile multiplication"
        (Right [Push (VInt 3), Push (VInt 4), BinOp Mul])
        (compileAST (AMul (AInt 3) (AInt 4)))

    , TestCase $ assertEqual "Compile division"
        (Right [Push (VInt 20), Push (VInt 4), BinOp Div])
        (compileAST (ADiv (AInt 20) (AInt 4)))

    , TestCase $ assertEqual "Compile complex expression (2 * 3 + 4)"
        (Right [Push (VInt 2), Push (VInt 3), BinOp Mul, Push (VInt 4), BinOp Add])
        (compileAST (AAdd (AMul (AInt 2) (AInt 3)) (AInt 4)))
    ]

-- Test comparison operations compilation
testComparisonCompilation :: Test
testComparisonCompilation = TestList
    [ TestCase $ assertEqual "Compile equality"
        (Right [Push (VInt 5), Push (VInt 5), BinOp Eq])
        (compileAST (AEq (AInt 5) (AInt 5)))

    , TestCase $ assertEqual "Compile less than"
        (Right [Push (VInt 3), Push (VInt 7), BinOp Less])
        (compileAST (ALess (AInt 3) (AInt 7)))

    , TestCase $ assertEqual "Compile variable comparison"
        (Right [LoadVar "x", Push (VInt 10), BinOp Eq])
        (compileAST (AEq (AVar "x") (AInt 10)))
    ]

-- Test if-else compilation
testIfElseCompilation :: Test
testIfElseCompilation = TestList
    [ TestCase $ assertBool "Compile if without else"
        (isRight (compileAST (AIf (ABool True) (ASeq [AInt 1]) (ASeq []))))

    , TestCase $ assertBool "Compile if with else"
        (isRight (compileAST (AIf (ABool True) (ASeq [AInt 1]) (ASeq [AInt 2]))))

    , TestCase $ case compileAST (AIf (ABool True) (ASeq [AInt 1]) (ASeq [AInt 2])) of
        Right code -> do
            -- Verify structure: condition, JumpIfFalse, then, Jump, else
            assertBool "Should contain JumpIfFalse"
                (any (\i -> case i of JumpIfFalse _ -> True; _ -> False) code)
            assertBool "Should contain Jump"
                (any (\i -> case i of Jump _ -> True; _ -> False) code)
        Left err -> assertFailure $ "Compilation failed: " ++ err
    ]

-- Test function compilation
testFunctionCompilation :: Test
testFunctionCompilation = TestList
    [ TestCase $ case compile [AFuncDef "add" ["a", "b"] (ASeq [AReturn (AAdd (AVar "a") (AVar "b"))]),
                               AFuncDef "main" [] (ASeq [AReturn (AInt 0)])] of
        Right (mainCode, funcs) -> do
            assertEqual "Main code should call main" [CallFunc "main" 0, Return] mainCode
            assertBool "Should have two functions" (Map.size funcs == 2)
            case Map.lookup "add" funcs of
                Nothing -> assertFailure "Function 'add' not found"
                Just (params, body) -> do
                    assertEqual "Should have 2 parameters" 2 (length params)
                    assertBool "Body should end with Return"
                        (not (null body) && last body == Return)
        Left err -> assertFailure $ "Compilation failed: " ++ err

    , TestCase $ case compile [AFuncDef "getZero" [] (ASeq [AReturn (AInt 0)]),
                               AFuncDef "main" [] (ASeq [AReturn (AInt 0)])] of
        Right (_, funcs) ->
            case Map.lookup "getZero" funcs of
                Nothing -> assertFailure "Function 'getZero' not found"
                Just (params, _) -> assertEqual "Should have 0 parameters" 0 (length params)
        Left err -> assertFailure $ "Compilation failed: " ++ err
    ]

-- Test function call compilation
testFunctionCallCompilation :: Test
testFunctionCallCompilation = TestList
    [ TestCase $ assertEqual "Compile function call with no arguments"
        (Right [CallFunc "foo" 0])
        (compileAST (ACall "foo" []))

    , TestCase $ assertEqual "Compile function call with one argument"
        (Right [Push (VInt 5), CallFunc "square" 1])
        (compileAST (ACall "square" [AInt 5]))

    , TestCase $ assertEqual "Compile function call with multiple arguments"
        (Right [Push (VInt 3), Push (VInt 7), CallFunc "add" 2])
        (compileAST (ACall "add" [AInt 3, AInt 7]))
    ]

-- Test return statement compilation
testReturnCompilation :: Test
testReturnCompilation = TestList
    [ TestCase $ assertEqual "Compile return with integer"
        (Right [Push (VInt 42), Return])
        (compileAST (AReturn (AInt 42)))

    , TestCase $ assertEqual "Compile return with expression"
        (Right [Push (VInt 1), Push (VInt 2), BinOp Add, Return])
        (compileAST (AReturn (AAdd (AInt 1) (AInt 2))))
    ]

-- Test multiple statements compilation
testMultipleStatementsCompilation :: Test
testMultipleStatementsCompilation = TestList
    [ TestCase $ assertEqual "Compile two assignments"
        (Right [Push (VInt 10), StoreVar "x", Push (VInt 20), StoreVar "y"])
        (compileAST (ASeq [AAssign "x" (AInt 10), AAssign "y" (AInt 20)]))

    , TestCase $ assertEqual "Compile assignment and expression"
        (Right [Push (VInt 5), StoreVar "x", LoadVar "x", Push (VInt 3), BinOp Add])
        (compileAST (ASeq [AAssign "x" (AInt 5), AAdd (AVar "x") (AInt 3)]))
    ]

-- Test function extraction
testFunctionExtraction :: Test
testFunctionExtraction = TestList
    [ TestCase $ do
        let ast = [AFuncDef "foo" [] (ASeq []), AInt 42, AFuncDef "bar" [] (ASeq [])]
        let (funcs, main) = extractFunctions ast
        assertEqual "Should extract 2 functions" 2 (length funcs)
        assertEqual "Main should have 1 statement" 1 (length main)

    , TestCase $ do
        let ast = [AInt 1, AInt 2, AInt 3]
        let (funcs, main) = extractFunctions ast
        assertEqual "Should extract 0 functions" 0 (length funcs)
        assertEqual "Main should have 3 statements" 3 (length main)
    ]

-- Test edge cases
testEdgeCases :: Test
testEdgeCases = TestList
    [ TestCase $ assertEqual "Compile empty sequence"
        (Right [])
        (compileAST (ASeq []))

    , TestCase $ assertBool "Compile nested sequences"
        (isRight (compileAST (ASeq [ASeq [AInt 1, AInt 2], AInt 3])))
    ]

-- Collect all compiler tests
compilerTests :: Test
compilerTests = TestLabel "Compiler Tests" $ TestList
    [ TestLabel "Literal Compilation" testLiteralCompilation
    , TestLabel "Variable Compilation" testVariableCompilation
    , TestLabel "Arithmetic Compilation" testArithmeticCompilation
    , TestLabel "Comparison Compilation" testComparisonCompilation
    , TestLabel "If-Else Compilation" testIfElseCompilation
    , TestLabel "Function Compilation" testFunctionCompilation
    , TestLabel "Function Call Compilation" testFunctionCallCompilation
    , TestLabel "Return Compilation" testReturnCompilation
    , TestLabel "Multiple Statements Compilation" testMultipleStatementsCompilation
    , TestLabel "Function Extraction" testFunctionExtraction
    , TestLabel "Edge Cases" testEdgeCases
    ]
