{-
-- EPITECH PROJECT, 2025
-- B-FUN-400-TLS-4-1-compressor-fabien.fraixanet [WSL: Debian]
-- File description:
-- KMeans
-}

module KMeans (kMeans) where

import Types
import System.Random (randomRIO)
import Data.List (minimumBy, foldl')
import Control.Parallel.Strategies (parMap, rdeepseq)
import qualified Data.Map.Strict as Map

colorDistance :: Color -> Color -> Double
colorDistance (r1, g1, b1) (r2, g2, b2) =
    sqrt $ fromIntegral $
        (r1 - r2) ^ (2 :: Int) +
        (g1 - g2) ^ (2 :: Int) +
        (b1 - b2) ^ (2 :: Int)

averageColor :: [Color] -> Color
averageColor colors =
  let n = fromIntegral (length colors) :: Double
      (sumR, sumG, sumB) = foldl' addColors (0, 0, 0) colors
  in (round (fromIntegral sumR / n),
      round (fromIntegral sumG / n),
      round (fromIntegral sumB / n))
  where
    addColors (r, g, b) (r', g', b') = (r + r', g + g', b + b')

initializeClusters :: Int -> [Pixel] -> IO [Cluster]
initializeClusters k pixelsList = do
  indices <- randomIndices k (length pixelsList)
  let selectedColors = map (snd . (pixelsList !!)) indices
  return $ map (\c -> Cluster c []) selectedColors

randomIndices :: Int -> Int -> IO [Int]
randomIndices n maxVal = go n []
  where
    go 0 acc = return acc
    go i acc = do
      r <- randomRIO (0, maxVal - 1)
      if r `elem` acc
        then go i acc
        else go (i - 1) (r : acc)

findClosestCluster :: [Cluster] -> Color -> Color
findClosestCluster clusters color =
  centroid $ minimumBy compareDistance clusters
  where
    compareDistance c1 c2 = compare
      (colorDistance color (centroid c1))
      (colorDistance color (centroid c2))

assignPixel :: [Cluster] -> Pixel -> (Color, Pixel)
assignPixel clusters pixel =
  let (_, color) = pixel
  in (findClosestCluster clusters color, pixel)

assignPixels :: [Cluster] -> [Pixel] -> [Cluster]
assignPixels clusters pixelsList =
  let emptyClusterMap = Map.fromList [(centroid c, []) | c <- clusters]
      pixelAssignments = parMap rdeepseq (assignPixel clusters) pixelsList
      clusterMap = foldl' insertAssignment emptyClusterMap pixelAssignments
  in [Cluster cent (clusterMap Map.! cent) | cent <- Map.keys clusterMap]
  where
    insertAssignment m (c, p) = Map.adjust (p:) c m

updateCentroids :: [Cluster] -> [Cluster]
updateCentroids = map updateCentroid
  where
    updateCentroid :: Cluster -> Cluster
    updateCentroid (Cluster _ pxs) =
      let colors = map snd pxs
          newCentroid = if null colors 
                         then (0, 0, 0)
                         else averageColor colors
      in Cluster newCentroid pxs

maxCentroidShift :: [Cluster] -> [Cluster] -> Double
maxCentroidShift oldClusters newClusters =
  maximum 
    [colorDistance (centroid old) (centroid new) 
    | (old, new) <- zip oldClusters newClusters]

kMeans :: Int -> Double -> [Pixel] -> IO [Cluster]
kMeans k convLimit pixelsList = 
  initializeClusters k pixelsList >>= iterateKMeans
  where
    iterateKMeans clusters =
      let assigned = assignPixels clusters pixelsList
          updated = updateCentroids assigned
          shift = maxCentroidShift clusters updated
          converged = (convLimit == 0 && shift == 0) || 
                     (convLimit > 0 && shift < convLimit)
      in if converged
         then pure updated
         else iterateKMeans updated