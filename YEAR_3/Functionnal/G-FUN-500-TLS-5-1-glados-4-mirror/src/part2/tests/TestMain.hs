{-
-- EPITECH PROJECT, 2025
-- GLaDOS Part 2
-- File description:
-- Main test runner
-}

module Main (main) where

import Test.HUnit
import System.Exit (exitFailure, exitSuccess)
import ParserTests
import CompilerTests
import VMTests
import IntegrationTests

-- Collect all test suites
allTests :: Test
allTests = TestList
    [ parserTests
    , compilerTests
    , vmTests
    , integrationTests
    ]

-- Main test runner
main :: IO ()
main = do
    putStrLn "========================================="
    putStrLn "  GLaDOS Part 2 - Test Suite"
    putStrLn "========================================="
    putStrLn ""

    testCounts <- runTestTT allTests

    putStrLn ""
    putStrLn "========================================="
    putStrLn "  Test Results Summary"
    putStrLn "========================================="
    putStrLn $ "Cases:    " ++ show (cases testCounts)
    putStrLn $ "Tried:    " ++ show (tried testCounts)
    putStrLn $ "Errors:   " ++ show (errors testCounts)
    putStrLn $ "Failures: " ++ show (failures testCounts)
    putStrLn "========================================="

    if errors testCounts + failures testCounts == 0
        then do
            putStrLn ""
            putStrLn "*** ALL TESTS PASSED ***"
            putStrLn ""
            exitSuccess
        else do
            putStrLn ""
            putStrLn "*** SOME TESTS FAILED ***"
            putStrLn ""
            exitFailure
