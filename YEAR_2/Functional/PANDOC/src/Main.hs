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
import Control.Exception (catch, IOException, SomeException)
import System.IO.Error (isDoesNotExistError)

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
printUsage = 
  mapM_ putStrLn
    [ "USAGE: ./mypandoc -i ifile -f oformat [-o ofile] [-e iformat]"
    , "    ifile    path to the file to convert"
    , "    oformat    output format (xml, json, markdown)"
    , "    ofile    path to the output file"
    , "    iformat    input format (xml, json, markdown)"
    ]

exitWithError :: String -> IO a
exitWithError msg = hPutStrLn stderr msg >> exitWith (ExitFailure 84)

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
detectInputFormat file = case reverse (takeWhile (/= '.') (reverse file)) of
  "xml" -> Just InXML
  "json" -> Just InJSON
  "md" -> Just InMarkdown
  "markdown" -> Just InMarkdown
  _ -> Nothing

checkMissingInputFile :: Options -> IO ()
checkMissingInputFile options = 
  when (isNothing (inputFile options)) 
    (exitWithError "Missing input file, use -i option")

checkMissingOutputFormat :: Options -> IO ()
checkMissingOutputFormat options =
  when (isNothing (outputFormat options))
    (exitWithError "Missing output format, use -f option")

inferInputFormat :: Options -> Options
inferInputFormat options = 
  case inputFormat options of
    Nothing -> case inputFile options of
      Just file -> options { inputFormat = detectInputFormat file }
      Nothing -> options
    Just _ -> options

checkUnknownInputFormat :: Options -> IO ()
checkUnknownInputFormat options =
  when (isNothing (inputFormat options))
    (exitWithError "Could not determine input format, use -e option")

validateOptions :: Options -> IO Options
validateOptions options = 
  checkMissingInputFile options >>
  checkMissingOutputFormat options >>
  let optionsWithInferred = inferInputFormat options in
  checkUnknownInputFormat optionsWithInferred >>
  return optionsWithInferred

parseDocument :: InputFormat -> String -> Maybe Document
parseDocument InXML content = 
  case runParser parseXMLDocument content of
    Just (doc, rest) -> 
      if null (filter (not . (`elem` " \t\n\r")) rest)
      then Just doc
      else Nothing
    Nothing -> Nothing
parseDocument InJSON content = 
  case runParser parseJSONDocument content of
    Just (doc, rest) -> 
      if null (filter (not . (`elem` " \t\n\r")) rest)
      then Just doc
      else Nothing
    Nothing -> Nothing
parseDocument InMarkdown content = 
  case runParser parseMarkdownDocument content of
    Just (doc, rest) -> 
      if null (filter (not . (`elem` " \t\n\r")) rest)
      then Just doc
      else Nothing
    Nothing -> Nothing

formatDocument :: OutputFormat -> Document -> String
formatDocument OutXML = documentToXML
formatDocument OutJSON = documentToJSON
formatDocument OutMarkdown = documentToMarkdown

safeReadFile :: String -> IO (Either String String)
safeReadFile path = catch (Right <$> readFile path) handler
  where
    handler :: IOException -> IO (Either String String)
    handler e
      | isDoesNotExistError e = return $ Left $ "File not found: " ++ path
      | otherwise = return $ Left $ "Error reading file: " ++ show e

processFile :: Options -> IO ()
processFile options =
  let Just inFile = inputFile options
      Just inFormat = inputFormat options
      Just outFormat = outputFormat options
  in
  processFileWithFormats inFile inFormat outFormat (outputFile options)

processFileWithFormats :: String -> InputFormat -> OutputFormat -> Maybe String -> IO ()
processFileWithFormats inFile inFormat outFormat outFileMaybe = do
  result <- safeReadFile inFile
  case result of
    Left err -> exitWithError err
    Right content -> parseAndFormat inFile inFormat outFormat content outFileMaybe

parseAndFormat :: String -> InputFormat -> OutputFormat -> String -> Maybe String -> IO ()
parseAndFormat inFile inFormat outFormat content outFileMaybe = 
  case parseDocument inFormat content of
    Nothing -> exitWithError $ 
                "Failed to parse " ++ inFile ++ " as " ++ show inFormat
    Just doc -> let formatted = formatDocument outFormat doc in
                outputResult formatted outFileMaybe

outputResult :: String -> Maybe String -> IO ()
outputResult formatted outFileMaybe = case outFileMaybe of
  Just outFile -> writeFile outFile formatted
  Nothing -> putStr formatted

main :: IO ()
main = getArgs >>= \args ->
  if null args
    then printUsage >> exitWith (ExitFailure 84)
    else parseArgs args >>= validateOptions >>= processFile