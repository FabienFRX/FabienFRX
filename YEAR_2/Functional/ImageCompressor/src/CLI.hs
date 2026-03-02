{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- CLI
-}

module CLI (parseArgs) where

import Types
import Options.Applicative

positiveIntReader :: ReadM Int
positiveIntReader = eitherReader $ \arg ->
    case reads arg of
        [(n, "")] -> if n <= 0 
                     then Left "Value n must be positive"
                     else Right n
        _         -> Left "Value n must be positive"

positiveDoubleReader :: ReadM Double
positiveDoubleReader = eitherReader $ \arg ->
    case reads arg of
        [(n, "")] -> if n < 0
                     then Left "Value must be positive"
                     else Right n
        _         -> Left "Invalid number"

nOption :: Parser Int
nOption = option positiveIntReader
    (short 'n'
    <> metavar "N"
    <> showDefault
    <> help "Number of colors in the final image")

lOption :: Parser Double
lOption = option positiveDoubleReader
    (short 'l'
    <> metavar "L"
    <> showDefault
    <> help "Convergence limit")

fOption :: Parser String
fOption = strOption
    (short 'f'
    <> metavar "F"
    <> showDefault
    <> help "Path to input file")

configParser :: Parser Config
configParser = Config
    <$> nOption
    <*> lOption
    <*> fOption

opts :: ParserInfo Config
opts = info (configParser <**> helper)
    (fullDesc 
    <> progDesc "Image compression using k-means clustering"
    <> header "Image Compressor - reduce image color palette")

parseArgs :: IO Config
parseArgs = execParser opts