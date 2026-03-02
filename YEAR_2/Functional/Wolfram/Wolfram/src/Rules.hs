{-
-- EPITECH PROJECT, 2025
-- Wolfram
-- File description:
-- Rules
-}

module Rules (Config(..), defaultConfig, generate) where

data Config = Config
    { ruleNumber    :: Int
    , startPosition :: Int
    , numberOfLines :: Int
    , windowSize    :: Int
    , moveOffset    :: Int
    , showGeneration :: Bool
    } deriving (Show)

defaultConfig :: Config
defaultConfig = Config (-1) 0 (-1) 80 0 False

generate :: Int -> [Int] -> [Int]
generate rule cells = map (applyRule rule) (windows 3 (0 : cells ++ [0]))

applyRule :: Int -> [Int] -> Int
applyRule rule [a, b, c] = (rule `div` (2 ^ (4 * a + 2 * b + c))) `mod` 2
applyRule _ _ = 0

windows :: Int -> [a] -> [[a]]
windows n xs = take (length xs - n + 1) (map (take n) (tails xs))

tails :: [a] -> [[a]]
tails [] = []
tails xs@(_:xs') = xs : tails xs'
