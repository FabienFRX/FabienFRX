{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- Lib
-}

module Lib
  ( runCompressor
  , displayUsage
  ) where

import Types
import Parser (parsePixels, ParseError(..))
import KMeans (kMeans)
import Output (formatOutput)
import Control.Exception (try, IOException)
import System.Exit (exitWith, ExitCode(ExitFailure))

runCompressor :: Config -> IO ()
runCompressor config = do
  pixelsResult <- readPixelsFromFile config
  case pixelsResult of
    Left err -> handleIOError err
    Right content -> processPixels config content

readPixelsFromFile :: Config -> IO (Either IOException String)
readPixelsFromFile config = 
  try (Prelude.readFile (inputFile config))

handleIOError :: IOException -> IO ()
handleIOError _ = handleError "Error: file does not exist"

handleError :: String -> IO ()
handleError msg = putStrLn msg >> exitWith (ExitFailure 84)

processPixels :: Config -> String -> IO ()
processPixels config content =
  case parsePixels content of
    Left InvalidRGBRange -> 
      handleError "Error: RGB values must be between 0 and 255"
    Left InvalidFormat -> 
      handleError "Error: invalid input format"
    Right pixelList ->
      case validatePixels pixelList of
        Nothing -> reportEmptyInput
        Just validPixels -> compressPixels config validPixels

validatePixels :: [Pixel] -> Maybe [Pixel]
validatePixels pixelList = 
  if null pixelList then Nothing else Just pixelList

reportEmptyInput :: IO ()
reportEmptyInput = 
  handleError "No valid pixels found in the input file"

compressPixels :: Config -> [Pixel] -> IO ()
compressPixels config pixelList = do
  clusters <- kMeans (numClusters config) 
                    (convergenceLimit config) 
                    pixelList
  putStr $ formatOutput clusters

displayUsage :: IO ()
displayUsage = putStr $ unlines
  [ "USAGE: ./imageCompressor -n N -l L -f F"
  , ""
  , "    N    number of colors in the final image"
  , "    L    convergence limit"
  , "    F    path to the file containing the colors of the pixels"]