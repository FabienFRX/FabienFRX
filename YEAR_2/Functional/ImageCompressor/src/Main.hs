{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- Main
-}

module Main (main) where

import Lib (runCompressor, displayUsage)
import CLI (parseArgs)
import Control.Exception (try, SomeException)
import System.Exit (exitWith, ExitCode(ExitFailure))
import System.Environment (getArgs)

main :: IO ()
main = do
  args <- getArgs
  handleArgs args

handleArgs :: [String] -> IO ()
handleArgs [] = displayUsage >> exitWith (ExitFailure 84)
handleArgs _ = handleCompression

handleCompression :: IO ()
handleCompression = do
  result <- try compressImage :: IO (Either SomeException ())
  case result of
    Left _ -> exitWith (ExitFailure 84)
    Right _ -> return ()

compressImage :: IO ()
compressImage = parseArgs >>= runCompressor