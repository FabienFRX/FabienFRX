{-
-- EPITECH PROJECT, 2025
-- Wolfram
-- File description:
-- Main
-}

module Main (main) where

import System.Environment (getArgs)
import Display (printGenerations, usage)
import ArgsParser (parseArgs)
import Rules (generate, Config(..))

main :: IO ()
main = do
    args <- getArgs
    handleArgs args

handleArgs :: [String] -> IO ()
handleArgs args
    | null args = putStrLn usage
    | otherwise = case parseArgs args of
        Left err -> putStrLn err
        Right rawConfig -> runSimulation (fillDefaultConfig rawConfig)

runSimulation :: Config -> IO ()
runSimulation config =
    let initialGen = replicate (startPosition config) 0 
                    ++ [1] 
                    ++ replicate (windowSize config) 0

        generations = take (numberOfLines config) 
                    $ iterate (generate (ruleNumber config)) initialGen
    in printGenerations (windowSize config) 
                        (moveOffset config) 
                        generations

fillDefaultConfig :: Config -> Config
fillDefaultConfig config = config
    { startPosition = if startPosition config == 0 then 40 else 
            startPosition config
    , numberOfLines = if numberOfLines config == 0 then 20 else
            numberOfLines config
    , windowSize = if windowSize config == 0 then 80 else
            windowSize config  -- Par défaut : 80
    , moveOffset = moveOffset config  -- `moveOffset` est laissé tel quel
    }
