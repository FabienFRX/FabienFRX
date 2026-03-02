{-
-- EPITECH PROJECT, 2025
-- my_pandoc
-- File description:
-- Main
-}

module Main (main) where

import System.Environment (getArgs)
import System.Exit (exitWith, ExitCode(..))
import System.IO (hPutStrLn, stderr)
import Data.Maybe (isNothing)
import Control.Monad (when)
import Parser
import Document hiding (FormatType(..))
import XMLParser (parseXMLDocument)
import JSONParser (parseJSONDocument)
import MarkdownParser (parseMarkdownDocument)
import XMLPrinter (documentToXML)
import JSONPrinter (documentToJSON)
import MarkdownPrinter (documentToMarkdown)
import qualified System.IO as SysIO (readFile, writeFile)

data InputFormat = InXML | InJSON | InMarkdown deriving (Show, Eq)
data OutputFormat = OutXML | OutJSON | OutMarkdown deriving (Show, Eq)

data Options = Options
  { inputFile :: Maybe String
  , outputFile :: Maybe String
  , inputFormat :: Maybe InputFormat
  , outputFormat :: Maybe OutputFormat
  } deriving (Show)

emptyOptions :: Options
emptyOptions = Options
  { inputFile = Nothing
  , outputFile = Nothing
  , inputFormat = Nothing
  , outputFormat = Nothing
  }

printUsage :: IO ()
printUsage = do
  putStrLn "USAGE: ./mypandoc -i ifile -f oformat [-o ofile] [-e iformat]"
  putStrLn "    ifile    path to the file to convert"
  putStrLn "    oformat    output format (xml, json, markdown)"
  putStrLn "    ofile    path to the output file"
  putStrLn "    iformat    input format (xml, json, markdown)"

exitWithError :: String -> IO a
exitWithError msg = do
  hPutStrLn stderr msg
  exitWith (ExitFailure 84)

parseArgs :: [String] -> IO Options
parseArgs [] = return emptyOptions
parseArgs ("-i":file:rest) = do
  options <- parseArgs rest
  return options { inputFile = Just file }
parseArgs ("-o":file:rest) = do
  options <- parseArgs rest
  return options { outputFile = Just file }
parseArgs ("-f":format:rest) = do
  options <- parseArgs rest
  case parseOutputFormat format of
    Just fmt -> return options { outputFormat = Just fmt }
    Nothing -> exitWithError $ "Invalid output format: " ++ format
parseArgs ("-e":format:rest) = do
  options <- parseArgs rest
  case parseInputFormat format of
    Just fmt -> return options { inputFormat = Just fmt }
    Nothing -> exitWithError $ "Invalid input format: " ++ format
parseArgs (arg:_) = exitWithError $ "Unknown argument: " ++ arg

parseInputFormat :: String -> Maybe InputFormat
parseInputFormat "xml" = Just InXML
parseInputFormat "json" = Just InJSON
parseInputFormat "markdown" = Just InMarkdown
parseInputFormat _ = Nothing

parseOutputFormat :: String -> Maybe OutputFormat
parseOutputFormat "xml" = Just OutXML
parseOutputFormat "json" = Just OutJSON
parseOutputFormat "markdown" = Just OutMarkdown
parseOutputFormat _ = Nothing

detectInputFormat :: String -> Maybe InputFormat
detectInputFormat file
  | hasXmlExt file = Just InXML
  | hasJsonExt file = Just InJSON
  | hasMarkdownExt file = Just InMarkdown
  | otherwise = Nothing
  where
    hasXmlExt = extension ".xml"
    hasJsonExt = extension ".json"
    hasMarkdownExt f = extension ".md" f || extension ".markdown" f

extension :: String -> String -> Bool
extension suffix str = 
  let suffixLength = length suffix
      strLength = length str
  in suffixLength <= strLength && 
     drop (strLength - suffixLength) str == suffix

checkRequiredInput :: Options -> IO ()
checkRequiredInput options = 
  when (isNothing (inputFile options)) $
    exitWithError "Missing input file, use -i option"

checkRequiredFormat :: Options -> IO ()
checkRequiredFormat options =
  when (isNothing (outputFormat options)) $
    exitWithError "Missing output format, use -f option"

inferFormat :: Options -> Options
inferFormat options = case (inputFormat options, inputFile options) of
  (Nothing, Just file) -> options { inputFormat = detectInputFormat file }
  _ -> options

checkFinalFormat :: Options -> IO ()
checkFinalFormat options =
  when (isNothing (inputFormat options)) $
    exitWithError "Could not determine input format, use -e option"

validateOptions :: Options -> IO Options
validateOptions options = do
  checkRequiredInput options
  checkRequiredFormat options
  let optionsWithInferred = inferFormat options
  checkFinalFormat optionsWithInferred
  return optionsWithInferred

parseDocument :: InputFormat -> String -> Maybe Document
parseDocument InXML content = case runParser parseXMLDocument content of
  Just (doc, _) -> Just doc
  Nothing -> Nothing
parseDocument InJSON content = case runParser parseJSONDocument content of
  Just (doc, _) -> Just doc
  Nothing -> Nothing
parseDocument InMarkdown content = case runParser parseMarkdownDocument content of
  Just (doc, _) -> Just doc
  Nothing -> Nothing

formatDocument :: OutputFormat -> Document -> String
formatDocument OutXML = documentToXML
formatDocument OutJSON = documentToJSON
formatDocument OutMarkdown = documentToMarkdown

getInputFile :: Options -> String
getInputFile options = case inputFile options of
  Just file -> file
  Nothing -> error "No input file specified"

getInputFormat :: Options -> InputFormat
getInputFormat options = case inputFormat options of
  Just format -> format
  Nothing -> error "No input format specified"

getOutputFormat :: Options -> OutputFormat
getOutputFormat options = case outputFormat options of
  Just format -> format
  Nothing -> error "No output format specified"

getFormatOptions :: Options -> (String, InputFormat, OutputFormat)
getFormatOptions options = 
  (getInputFile options, 
   getInputFormat options,
   getOutputFormat options)

handleOutput :: Maybe String -> String -> IO ()
handleOutput outFile result = case outFile of
  Just file -> SysIO.writeFile file result
  Nothing -> putStr result

handleParseError :: String -> InputFormat -> IO a
handleParseError inFile inFormat = exitWithError $ 
  "Failed to parse " ++ inFile ++ 
  " as " ++ show inFormat

processFile :: Options -> IO ()
processFile options = do
  let (inFile, inFormat, outFormat) = getFormatOptions options
  content <- SysIO.readFile inFile
  case parseDocument inFormat content of
    Nothing -> handleParseError inFile inFormat
    Just doc -> handleOutput (outputFile options) (formatDocument outFormat doc)

main :: IO ()
main = do
  args <- getArgs
  if null args
    then printUsage >> exitWith (ExitFailure 84)
    else do
      options <- parseArgs args >>= validateOptions
      processFile options