{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- Types
-}

module Types 
  ( Point
  , Color
  , Pixel
  , Cluster(..)
  , Config(..)
  ) where

type Point = (Int, Int)

type Color = (Int, Int, Int)

type Pixel = (Point, Color)

data Cluster = Cluster
  { centroid :: Color
  , pixels   :: [Pixel]
  } deriving (Show, Eq)

data Config = Config
  { numClusters :: Int
  , convergenceLimit :: Double
  , inputFile :: FilePath
  } deriving (Show)