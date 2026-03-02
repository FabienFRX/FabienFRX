{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- Output
-}

module Output (formatOutput) where

import Types

formatPoint :: Point -> String
formatPoint (x, y) = "(" ++ show x ++ "," ++ show y ++ ")"

formatColor :: Color -> String
formatColor (r, g, b) = "(" ++ show r ++ "," ++ show g ++ "," ++ show b ++ ")"

formatCluster :: Cluster -> String
formatCluster (Cluster cent pxs) =
  "--\n" ++
  formatColor cent ++
  "\n-\n" ++
  concatMap formatPixel pxs
  where
    formatPixel (point, color) = 
      formatPoint point ++ " " ++ formatColor color ++ "\n"

formatOutput :: [Cluster] -> String
formatOutput = concatMap formatCluster