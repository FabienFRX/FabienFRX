{-
-- EPITECH PROJECT, 2025
-- Wolfram
-- File description:
-- ArgsParser
-}
module ArgsParser (parseArgs) where

import Rules (Config(..), defaultConfig)
import Text.Read (readMaybe)

parseArgs :: [String] -> Either String Config
parseArgs [] = Left "Error: --rule is mandatory!"
parseArgs args = parseArgsHelper args defaultConfig

parseRule :: String -> [String] -> Config -> Either String Config
parseRule value rest cfg =
    case readMaybe value of
        Just n | n >= 0 && n <= 255 -> parseArgsHelper rest cfg
            { ruleNumber = n }
        _ -> Left $ "Invalid value for --rule: " ++ value

parseStart :: String -> [String] -> Config -> Either String Config
parseStart value rest cfg =
    case readMaybe value of
        Just n | n >= 0 -> parseArgsHelper rest cfg { startPosition = n }
        _ -> Left $ "Invalid value for --start: " ++ value

parseLines :: String -> [String] -> Config -> Either String Config
parseLines value rest cfg =
    case readMaybe value of
        Just n | n >= (-1) -> parseArgsHelper rest cfg { numberOfLines = n }
        _ -> Left $ "Invalid value for --lines: " ++ value

parseWindow :: String -> [String] -> Config -> Either String Config
parseWindow value rest cfg =
    case readMaybe value of
        Just n | n >= 1 -> parseArgsHelper rest cfg { windowSize = n }
        _ -> Left $ "Invalid value for --window: " ++ value

parseMove :: String -> [String] -> Config -> Either String Config
parseMove value rest cfg =
    case readMaybe value of
        Just n -> parseArgsHelper rest cfg { moveOffset = n }
        _ -> Left $ "Invalid value for --move: " ++ value

parseArgsHelper :: [String] -> Config -> Either String Config
parseArgsHelper [] cfg
    | ruleNumber cfg == -1 = Left "Error: --rule is mandatory!"
    | otherwise = Right cfg
parseArgsHelper ("--help":_) _ = 
    Left ("Usage: --rule <value> [--start <value>] [--lines <value>]" ++
        "[--window <value>] [--move <value>] [--generation]")
parseArgsHelper ("--generation":rest) cfg = parseArgsHelper rest cfg
    { showGeneration = True }
parseArgsHelper ("--rule":value:rest) cfg = parseRule value rest cfg
parseArgsHelper ("--rule":[]) _ = Left "Missing value for --rule"
parseArgsHelper ("--start":value:rest) cfg = parseStart value rest cfg
parseArgsHelper ("--start":[]) _ = Left "Missing value for --start"
parseArgsHelper ("--lines":value:rest) cfg = parseLines value rest cfg
parseArgsHelper ("--lines":[]) _ = Left "Missing value for --lines"
parseArgsHelper ("--window":value:rest) cfg = parseWindow value rest cfg
parseArgsHelper ("--window":[]) _ = Left "Missing value for --window"
parseArgsHelper ("--move":value:rest) cfg = parseMove value rest cfg
parseArgsHelper ("--move":[]) _ = Left "Missing value for --move"
parseArgsHelper (flag:_) _ = Left $ "Unknown option: " ++ flag
