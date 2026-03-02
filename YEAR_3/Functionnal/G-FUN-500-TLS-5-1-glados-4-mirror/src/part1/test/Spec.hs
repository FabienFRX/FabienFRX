{-
-- EPITECH PROJECT, 2025
-- G-FUN-500-TLS-5-1-glados-4
-- File description:
-- Spec
-}

import Test.Hspec
import Parser
import AST
import Evaluator

main :: IO ()
main = hspec $ do
    describe "Parser" $ do
        describe "Atoms" $ do
            it "parses positive integers" $ do
                parseProgram "42" `shouldBe` Right [SInt 42]
            
            it "parses negative integers" $ do
                parseProgram "-42" `shouldBe` Right [SInt (-42)]
            
            it "parses boolean true" $ do
                parseProgram "#t" `shouldBe` Right [SBool True]
            
            it "parses boolean false" $ do
                parseProgram "#f" `shouldBe` Right [SBool False]
            
            it "parses symbols" $ do
                parseProgram "foo" `shouldBe` Right [SSymbol "foo"]
                parseProgram "+" `shouldBe` Right [SSymbol "+"]
                parseProgram "eq?" `shouldBe` Right [SSymbol "eq?"]
        
        describe "Lists" $ do
            it "parses empty list" $ do
                parseProgram "()" `shouldBe` Right [SList []]
            
            it "parses simple list" $ do
                parseProgram "(1 2 3)" `shouldBe` 
                    Right [SList [SInt 1, SInt 2, SInt 3]]
            
            it "parses nested lists" $ do
                parseProgram "((1 2) (3 4))" `shouldBe`
                    Right [SList [SList [SInt 1, SInt 2], SList [SInt 3, SInt 4]]]
            
            it "parses function calls" $ do
                parseProgram "(+ 1 2)" `shouldBe`
                    Right [SList [SSymbol "+", SInt 1, SInt 2]]
        
        describe "Multiple expressions" $ do
            it "parses multiple expressions" $ do
                parseProgram "1 2 3" `shouldBe`
                    Right [SInt 1, SInt 2, SInt 3]

        describe "Edge cases and error handling" $ do
            it "handles whitespace variations" $ do
                parseProgram "  (  +   1   2  )  " `shouldBe`
                    Right [SList [SSymbol "+", SInt 1, SInt 2]]
            
            it "parses complex symbols" $ do
                parseProgram "symbol-with-dashes" `shouldBe` Right [SSymbol "symbol-with-dashes"]
                parseProgram "symbol_with_underscores" `shouldBe` Right [SSymbol "symbol_with_underscores"]
                parseProgram "symbol?" `shouldBe` Right [SSymbol "symbol?"]
            
            it "handles empty input" $ do
                parseProgram "" `shouldBe` Right []
            
            it "parses multiple nested expressions" $ do
                isRight (parseProgram "(define x 5) (define y (+ x 1)) y") `shouldBe` True

    describe "AST Conversion" $ do
        it "converts integers" $ do
            sexprToAST (SInt 42) `shouldBe` Right (AInt 42)
        
        it "converts booleans" $ do
            sexprToAST (SBool True) `shouldBe` Right (ABool True)
        
        it "converts symbols" $ do
            sexprToAST (SSymbol "x") `shouldBe` Right (ASymbol "x")
        
        it "converts define expressions" $ do
            sexprToAST (SList [SSymbol "define", SSymbol "x", SInt 5]) `shouldBe`
                Right (Define "x" (AInt 5))
        
        it "converts lambda expressions" $ do
            sexprToAST (SList [SSymbol "lambda", SList [SSymbol "x"], SSymbol "x"]) `shouldBe`
                Right (Lambda ["x"] (ASymbol "x"))
        
        it "converts if expressions" $ do
            sexprToAST (SList [SSymbol "if", SBool True, SInt 1, SInt 2]) `shouldBe`
                Right (If (ABool True) (AInt 1) (AInt 2))
        
        it "converts function calls" $ do
            sexprToAST (SList [SSymbol "+", SInt 1, SInt 2]) `shouldBe`
                Right (Call (ASymbol "+") [AInt 1, AInt 2])

        describe "Advanced AST conversions" $ do
            it "converts function definition with parameters" $ do
                let expr = SList [SSymbol "define", SList [SSymbol "add", SSymbol "x", SSymbol "y"], 
                                  SList [SSymbol "+", SSymbol "x", SSymbol "y"]]
                sexprToAST expr `shouldBe` 
                    Right (Define "add" (Lambda ["x", "y"] (Call (ASymbol "+") [ASymbol "x", ASymbol "y"])))
            
            it "handles nested function calls" $ do
                let expr = SList [SSymbol "+", SInt 1, SList [SSymbol "*", SInt 2, SInt 3]]
                sexprToAST expr `shouldBe`
                    Right (Call (ASymbol "+") [AInt 1, Call (ASymbol "*") [AInt 2, AInt 3]])
            
            it "rejects empty lists" $ do
                sexprToAST (SList []) `shouldBe` Left "Empty list is not a valid expression"
            
            it "rejects invalid parameter lists" $ do
                let expr = SList [SSymbol "lambda", SList [SInt 42], SSymbol "x"]
                sexprToAST expr `shouldBe` Left "Expected symbol in parameter list"

    describe "Evaluator" $ do
        describe "Literals" $ do
            it "evaluates integers" $ do
                eval initialEnv (AInt 42) `shouldBe` Right (VInt 42, initialEnv)
            
            it "evaluates booleans" $ do
                eval initialEnv (ABool True) `shouldBe` Right (VBool True, initialEnv)
        
        describe "Arithmetic operations" $ do
            it "evaluates addition" $ do
                let ast = Call (ASymbol "+") [AInt 1, AInt 2]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 3)
            
            it "evaluates subtraction" $ do
                let ast = Call (ASymbol "-") [AInt 5, AInt 3]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 2)
            
            it "evaluates multiplication" $ do
                let ast = Call (ASymbol "*") [AInt 6, AInt 7]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 42)
            
            it "evaluates division" $ do
                let ast = Call (ASymbol "div") [AInt 10, AInt 2]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 5)
            
            it "evaluates modulo" $ do
                let ast = Call (ASymbol "mod") [AInt 10, AInt 3]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 1)
            
            it "handles division by zero" $ do
                let ast = Call (ASymbol "div") [AInt 10, AInt 0]
                eval initialEnv ast `shouldBe` Left "*** ERROR: division by zero"
            
            it "handles modulo by zero" $ do
                let ast = Call (ASymbol "mod") [AInt 10, AInt 0]
                eval initialEnv ast `shouldBe` Left "*** ERROR: modulo by zero"
        
        describe "Comparison operations" $ do
            it "evaluates eq? on integers" $ do
                let ast = Call (ASymbol "eq?") [AInt 5, AInt 5]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VBool True)
            
            it "evaluates eq? on booleans" $ do
                let ast = Call (ASymbol "eq?") [ABool True, ABool True]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VBool True)
            
            it "evaluates < operator" $ do
                let ast = Call (ASymbol "<") [AInt 3, AInt 5]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VBool True)
            
            it "evaluates < operator with false result" $ do
                let ast = Call (ASymbol "<") [AInt 5, AInt 3]
                fmap fst (eval initialEnv ast) `shouldBe` Right (VBool False)
        
        describe "Variable binding" $ do
            it "defines and uses variables" $ do
                let env1 = initialEnv
                case eval env1 (Define "x" (AInt 42)) of
                    Right (_, env2) -> 
                        eval env2 (ASymbol "x") `shouldBe` Right (VInt 42, env2)
                    _ -> expectationFailure "Define should succeed"
            
            it "reports unbound variables" $ do
                eval initialEnv (ASymbol "undefined") `shouldBe` 
                    Left "*** ERROR: variable undefined is not bound."
            
            it "handles variable shadowing" $ do
                let env1 = initialEnv
                case eval env1 (Define "x" (AInt 1)) of
                    Right (_, env2) -> 
                        case eval env2 (Define "x" (AInt 2)) of
                            Right (_, env3) -> 
                                fmap fst (eval env3 (ASymbol "x")) `shouldBe` Right (VInt 2)
                            _ -> expectationFailure "Second define should succeed"
                    _ -> expectationFailure "First define should succeed"
        
        describe "Conditionals" $ do
            it "evaluates if with true condition" $ do
                let ast = If (ABool True) (AInt 1) (AInt 2)
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 1)
            
            it "evaluates if with false condition" $ do
                let ast = If (ABool False) (AInt 1) (AInt 2)
                fmap fst (eval initialEnv ast) `shouldBe` Right (VInt 2)
            
            it "errors when condition is not boolean" $ do
                let ast = If (AInt 42) (AInt 1) (AInt 2)
                eval initialEnv ast `shouldBe` Left "*** ERROR: if condition must be a boolean"
        
        describe "Lambda expressions" $ do
            it "creates and applies lambda" $ do
                let lambda = Lambda ["x"] (ASymbol "x")
                    call = Call lambda [AInt 42]
                fmap fst (eval initialEnv call) `shouldBe` Right (VInt 42)
            
            it "creates lambda with multiple parameters" $ do
                let lambda = Lambda ["x", "y"] (Call (ASymbol "+") [ASymbol "x", ASymbol "y"])
                    call = Call lambda [AInt 3, AInt 4]
                fmap fst (eval initialEnv call) `shouldBe` Right (VInt 7)
            
            it "handles closures correctly" $ do
                let env1 = initialEnv
                case eval env1 (Define "a" (AInt 10)) of
                    Right (_, env2) -> 
                        case eval env2 (Lambda ["x"] (Call (ASymbol "+") [ASymbol "x", ASymbol "a"])) of
                            Right (VProcedure _ _ closureEnv, _) ->
                                case eval closureEnv (Call (ASymbol "+") [AInt 5, ASymbol "a"]) of
                                    Right (VInt 15, _) -> return ()
                                    other -> expectationFailure $ "Closure failed: " ++ show other
                            _ -> expectationFailure "Lambda creation failed"
                    _ -> expectationFailure "Define failed"

        describe "Error handling" $ do
            it "rejects calling non-procedures" $ do
                let ast = Call (AInt 42) []
                eval initialEnv ast `shouldBe` Left "*** ERROR: attempting to call a non-procedure"
            
            it "rejects wrong number of arguments to builtins" $ do
                let ast = Call (ASymbol "+") [AInt 1]
                eval initialEnv ast `shouldBe` Left "*** ERROR: invalid arguments for builtin + (got 1 arguments)"
            
            it "rejects wrong argument types to builtins" $ do
                let ast = Call (ASymbol "+") [ABool True, AInt 1]
                eval initialEnv ast `shouldBe` Left "*** ERROR: invalid arguments for builtin + (got 2 arguments)"
            
            it "handles recursive function definitions" $ do
                let factDef = Define "fact" 
                            (Lambda ["n"] 
                                (If (Call (ASymbol "eq?") [ASymbol "n", AInt 0])
                                    (AInt 1)
                                    (Call (ASymbol "*") 
                                        [ASymbol "n", 
                                         Call (ASymbol "fact") 
                                            [Call (ASymbol "-") [ASymbol "n", AInt 1]]])))
                case eval initialEnv factDef of
                    Right (_, env2) -> 
                        case eval env2 (Call (ASymbol "fact") [AInt 5]) of
                            Right (VInt 120, _) -> return ()
                            other -> expectationFailure $ "Recursion failed: " ++ show other
                    _ -> expectationFailure "Function definition failed"

        describe "Environment and sequencing" $ do
            it "maintains environment across evaluations" $ do
                let asts = [Define "x" (AInt 10), Define "y" (AInt 20), Call (ASymbol "+") [ASymbol "x", ASymbol "y"]]
                case evalSequence initialEnv asts of
                    Right (VInt 30, _) -> return ()
                    other -> expectationFailure $ "Sequencing failed: " ++ show other
            
            it "handles nested scopes correctly" $ do
                let innerLambda = Lambda ["x"] (Call (ASymbol "+") [ASymbol "x", ASymbol "outer"])
                    outerDef = Define "outer" (AInt 100)
                    call = Call innerLambda [AInt 42]
                case evalSequence initialEnv [outerDef, call] of
                    Right (VInt 142, _) -> return ()
                    other -> expectationFailure $ "Nested scopes failed: " ++ show other

    describe "Integration tests" $ do
        it "evaluates factorial function" $ do
            let program = "(define (fact n) (if (eq? n 0) 1 (* n (fact (- n 1))))) (fact 5)"
            case parseProgram program of
                Right sexprs -> case mapM sexprToAST sexprs of
                    Right [def, call] -> case eval initialEnv def of
                        Right (_, env2) -> case eval env2 call of
                            Right (VInt 120, _) -> return ()
                            other -> expectationFailure $ "Expected 120, got: " ++ show other
                        Left err -> expectationFailure err
                    Right other -> expectationFailure $ "Expected exactly 2 expressions, got: " ++ show (length other)
                    Left err -> expectationFailure err
                Left err -> expectationFailure $ show err

        it "handles complex nested expressions" $ do
            let program = "(define a 10) (define b 20) (+ a (* b 2))"
            case parseAndConvert program of
                Right asts -> 
                    case evalSequence initialEnv asts of
                        Right (VInt 50, _) -> return ()
                        Right (other, _) -> expectationFailure $ "Expected VInt 50, got: " ++ show other
                        Left err -> expectationFailure $ "Evaluation failed: " ++ err
                Left err -> expectationFailure $ "Parse failed: " ++ err
        
        it "evaluates boolean expressions" $ do
            let program = "(define x 5) (define y 10) (< x y)"
            case parseAndConvert program of
                Right asts -> 
                    case evalSequence initialEnv asts of
                        Right (VBool True, _) -> return ()
                        Right (other, _) -> expectationFailure $ "Expected VBool True, got: " ++ show other
                        Left err -> expectationFailure $ "Evaluation failed: " ++ err
                Left err -> expectationFailure $ "Parse failed: " ++ err
        
        it "handles lambda applications" $ do
            let program = "((lambda (x y) (+ x y)) 3 4)"
            case parseAndConvert program of
                Right [ast] -> 
                    case eval initialEnv ast of
                        Right (VInt 7, _) -> return ()
                        Right (other, _) -> expectationFailure $ "Expected VInt 7, got: " ++ show other
                        Left err -> expectationFailure $ "Evaluation failed: " ++ err
                Right [] -> expectationFailure "Expected one AST, got empty list"
                Right (_:_:_) -> expectationFailure "Expected exactly one AST, got multiple"
                Left err -> expectationFailure $ "Parse failed: " ++ err

    describe "Value formatting" $ do
        it "formats integers correctly" $ do
            valueToString (VInt 42) `shouldBe` "42"
        
        it "formats booleans correctly" $ do
            valueToString (VBool True) `shouldBe` "#t"
            valueToString (VBool False) `shouldBe` "#f"
        
        it "formats procedures correctly" $ do
            valueToString (VProcedure [] (AInt 0) []) `shouldBe` "#<procedure>"
        
        it "formats builtins correctly" $ do
            valueToString (VBuiltin "+") `shouldBe` "#<builtin:+>"
        
        it "filters out unit values in results" $ do
            let values = [VUnit, VInt 42, VUnit, VBool True]
            let filteredResults = filter (not . isUnit) values
            unlines (map valueToString filteredResults) `shouldBe` "42\n#t\n"

  where
    isRight :: Either a b -> Bool
    isRight (Right _) = True
    isRight (Left _) = False

    isUnit :: Value -> Bool
    isUnit VUnit = True
    isUnit _ = False