{-
-- EPITECH PROJECT, 2025
-- Wolfram
-- File description:
-- Display
-}

module Display (printGeneration, printGenerations, usage) where

-- Message d'utilisation affiché lorsque l'exécutable est lancé sans argument
usage :: String
usage = "USAGE:\n\t./wolfram --rule value [--start value] [--lines value]\
         \ [--window value] [--move value]\n\n--rule\t: select the rule to\
         \ apply\n--start\t: the first line of the generation to display\n\
         \--lines\t: the number of lines to display\n--window: the size\
         \ of the display window\n--move\t: translation to apply to the\
         \ window (right if positive, left if negative)"

-- Affiche une génération unique avec la bonne fenêtre et le déplacement
printGeneration :: Int -> Int -> [Int] -> IO ()
printGeneration window move gen = putStrLn $ formatGeneration window move gen

-- Applique la fenêtre et le décalage à une génération
formatGeneration :: Int -> Int -> [Int] -> String
formatGeneration window move gen =
    let len = length gen
        baseStart = (len - window) `div` 2  -- Centre l'affichage par défaut
        startIndex = max 0 (baseStart + move)  -- Applique le décalage
        endIndex = min len (startIndex + window)  -- Assure que la fenêtre ne déborde pas
        visibleGen = take (endIndex - startIndex) (drop startIndex gen)
    in map (\x -> if x == 1 then '*' else ' ') visibleGen

-- Affiche plusieurs générations avec la bonne fenêtre et le décalage
printGenerations :: Int -> Int -> [[Int]] -> IO ()
printGenerations window move generations = mapM_
    (putStrLn . formatGeneration window move)
    generations
