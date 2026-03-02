{-
-- EPITECH PROJECT, 2025
-- B-PDG-300-TLS-3-1-PDGD03-fabien.fraixanet
-- File description:
-- Tree
-}

data Tree a = Empty | Node (Tree a) a (Tree a)
    deriving (Show)

addInTree :: Ord a => a -> Tree a -> Tree a
addInTree newValue Empty = Node Empty newValue Empty
addInTree newValue (Node left currentVal right)
    | newValue < currentVal = Node (addInTree newValue left) currentVal right
    | newValue >= currentVal = Node left currentVal (addInTree newValue right)

instance Functor Tree where
    fmap _ Empty = Empty
    fmap f (Node left val right) = Node (fmap f left) (f val) (fmap f right)

listToTree :: Ord a => [a] -> Tree a
listToTree [] = Empty
listToTree (x:xs) = addInTree x (listToTree xs)

treeToList :: Tree a -> [a]
treeToList Empty = []
treeToList (Node left val right) = treeToList left ++ [val] ++ treeToList right

treeSort :: Ord a => [a] -> [a]
treeSort xs = treeToList (listToTree xs)

instance Foldable Tree where
    foldr _ acc Empty = acc
    foldr f acc (Node left a right) = foldr f (f a (foldr f acc right)) left