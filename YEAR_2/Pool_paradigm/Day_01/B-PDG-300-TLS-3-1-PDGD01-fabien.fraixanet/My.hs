{-
-- EPITECH PROJECT, 2025
-- Day_01
-- File description:
-- My
-}

mySucc :: Int -> Int
mySucc x = x + 1

myIsNeg :: Int -> Bool
myIsNeg x = x < 0

myAbs :: Int -> Int
myAbs x = if x < 0 then -x else x

myMin :: Int -> Int ->Int
myMin x y = if x < y then x else y

myMax :: Int -> Int -> Int
myMax x y = if x > y then x else y

myTuple :: a -> b -> (a, b)
myTuple x y = (x, y)

myTruple :: a -> b -> c -> (a, b, c)
myTruple x y z = (x, y, z)

myFst :: (a, b) -> a
myFst (x, y) = x

mySnd :: (a, b) -> b
mySnd (x, y) = y

mySwap :: (a, b) -> (b, a)
mySwap (x, y) = (y, x)

myHead :: [a] -> a
myHead [] = error "empty list"
myHead (x:_) = x

myTail :: [a] -> [a]
myTail [] = error "empty list"
myTail (x : y) = y

myLength :: [a] -> Int
myLength [] = 0
myLength (_:xs) = 1 + myLength xs

myNth :: [a] -> Int -> a
myNth []_ = error "out of range"
myNth (x : xs) 0 = x
myNth (_ : xs) n = myNth xs (n - 1)

myTake :: Int -> [a] -> [a]
myTake 0 _ = []
myTake _ [] = []
myTake n (x:xs) = x:myTake (n-1) xs

myDrop :: Int -> [a] -> [a]
myDrop 0 xs = xs
myDrop _ [] = []
myDrop n (x:xs) = myDrop (n - 1) xs

myAppend :: [a] -> [a] -> [a]
myAppend [] y = y
myAppend (x:xs) y = x : (myAppend xs y)

myReverse :: [a] -> [a]
myReverse [] = []
myReverse (x:xs) = myAppend (myReverse xs) [x]

myInit :: [a] -> [a]
myInit [] = error "empty list"
myInit [_] = []
myInit (x:xs) = x : myInit xs

myLast :: [a] -> a
myLast [] = error "empty list"
myLast [x] = x
myLast (_:xs) = myLast xs

myZip :: [a] -> [b] -> [(a, b)]
myZip []_ = []
myZip _[] = []
myZip (x:xs) (y:ys) = (x, y) : myZip xs ys

myUnzip :: [(a, b)] -> ([a], [b])
myUnzip [] = ([], [])
myUnzip ((x, y) : xs) = (x : myFst (myUnzip xs), y : mySnd (myUnzip xs))

myMap :: (a -> b) -> [a] -> [b]
myMap _[] = []
myMap f (x:xs) = f x : myMap f xs

myFilter :: (a -> Bool) -> [a] -> [a]
myFilter _[] = []
myFilter p (x:xs)
    | p x = x : myFilter p xs
    | otherwise = myFilter p xs

myFoldl :: (b -> a -> b) -> b -> [a] -> b
myFoldl _ acc [] = acc
myFoldl f acc (x:xs) = myFoldl f (f acc x) xs

myFoldr :: (a -> b -> b) -> b -> [a] -> b
myFoldr _ acc [] = acc
myFoldr f acc (x:xs) = f x (myFoldr f acc xs)

myPartition :: (a -> Bool) -> [a] -> ([a], [a])
myPartition _ [] = ([], [])
myPartition p (x:xs) = if p x
    then (x : myFst (myPartition p xs), mySnd (myPartition p xs))
    else (myFst (myPartition p xs), x : mySnd (myPartition p xs))

myQuickSort :: (a -> a -> Bool) -> [a] -> [a]
myQuickSort _ [] = []
myQuickSort f (x : xs) = myAppend
    (myQuickSort f (mySnd (myPartition (f x) xs)))
    (x : myQuickSort f (myFst(myPartition (f x) xs)))
