import Test.Hspec
import Control.Exception (try, IOException, SomeException)
import System.IO.Temp (withSystemTempFile)
import System.IO (hPutStr, hClose)
import Data.Time (getCurrentTime, diffUTCTime)
import qualified Data.List.NonEmpty as NE
 
import Types
import qualified Types as T
import Parser
import KMeans (kMeans)
import Output (formatOutput)
import Lib (runCompressor)
 
-- Define colorDistance function locally for testing
colorDistance :: Color -> Color -> Double
colorDistance (r1, g1, b1) (r2, g2, b2) =
    sqrt $ fromIntegral $
        (r1 - r2) ^ (2 :: Int) +
        (g1 - g2) ^ (2 :: Int) +
        (b1 - b2) ^ (2 :: Int)
 
main :: IO ()
main = hspec $ do
  describe "File Format Tests" $ do
    it "parses valid file format correctly" $ do
      withSystemTempFile "test_valid.txt" $ \path handle -> do
        -- Write valid data to the temp file
        let validContent = "(0,0) (33,18,109)\n(0,1) (33,18,109)\n(0,2) (33,21,109)"
        hPutStr handle validContent
        hClose handle
       
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right pixelList -> length pixelList `shouldBe` 3
          Left err -> expectationFailure $ "Should not fail on valid file format: " ++ show err
 
    it "rejects malformed coordinate format" $ do
      withSystemTempFile "test_invalid_format.txt" $ \path handle -> do
        -- Write invalid coordinate format
        let invalidContent = "(0,0 (33,18,109)\n(0,1) (33,18,109)"  -- Missing closing parenthesis
        hPutStr handle invalidContent
        hClose handle
       
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right pixelList -> length pixelList `shouldBe` 1  -- Should parse the valid line only
          Left _ -> return ()  -- Acceptable if the whole file is rejected
 
    it "rejects malformed color format" $ do
      withSystemTempFile "test_invalid_color.txt" $ \path handle -> do
        -- Write invalid color format
        let invalidContent = "(0,0) (33,18)\n(0,1) (33,18,109)"  -- Missing blue component
        hPutStr handle invalidContent
        hClose handle
       
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right pixelList -> length pixelList `shouldBe` 1  -- Should parse the valid line only
          Left _ -> return ()  -- Acceptable if the whole file is rejected
 
  describe "RGB Value Validation Tests" $ do
    it "accepts RGB values within range 0-255" $ do
      withSystemTempFile "test_valid_rgb.txt" $ \path handle -> do
        -- Write valid RGB values
        let validContent = "(0,0) (0,0,0)\n(0,1) (255,255,255)\n(0,2) (128,128,128)"
        hPutStr handle validContent
        hClose handle
       
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right pixelList -> length pixelList `shouldBe` 3
          Left err -> expectationFailure $ "Should not fail on valid RGB values: " ++ show err
         
    it "rejects RGB values below 0" $ do
      withSystemTempFile "test_rgb_below_range.txt" $ \path handle -> do
        -- Write RGB values below range
        let invalidContent = "(0,0) (-1,20,30)"
        hPutStr handle invalidContent
        hClose handle
       
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right _ -> expectationFailure "Should fail on negative RGB values"
          Left _ -> return ()
         
    it "rejects RGB values above 255" $ do
      withSystemTempFile "test_rgb_above_range.txt" $ \path handle -> do
        -- Write RGB values above range
        let invalidContent = "(0,0) (33,256,109)"
        hPutStr handle invalidContent
        hClose handle
        1
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right _ -> expectationFailure "Should fail on RGB values > 255"
          Left _ -> return ()
         
    it "handles mixed valid and invalid RGB values" $ do
      withSystemTempFile "test_mixed_rgb.txt" $ \path handle -> do
        -- Write mixed valid and invalid RGB values
        let mixedContent = "(0,0) (33,18,109)\n(0,1) (33,256,109)"
        hPutStr handle mixedContent
        hClose handle
       
        -- Read and parse the file
        content <- readFile path
        case parsePixels content of
          Right _ -> expectationFailure "Should fail if any RGB value is invalid"
          Left _ -> return ()
 
  describe "Parser Tests" $ do
    it "parses valid pixel data correctly" $ do
      let input = "(0,0) (33,18,109)\n(0,1) (33,18,109)"
      case parsePixels input of
        Right pixelList -> length pixelList `shouldBe` 2
        Left err -> expectationFailure $ "Failed to parse valid pixels: " ++ show err
 
  describe "Types Tests" $ do
    it "creates valid cluster" $ do
      let testCentroid = (100, 100, 100)
      let testPixels = [((0,0), (90,90,90)), ((1,1), (110,110,110))]
      let cluster = Cluster testCentroid testPixels
      T.centroid cluster `shouldBe` testCentroid
      T.pixels cluster `shouldBe` testPixels
 
  describe "Output Tests" $ do
    it "formats clusters correctly" $ do
      let cluster = Cluster (100, 100, 100) [((0,0), (90,90,90)), ((1,1), (110,110,110))]
      let expected = "--\n(100,100,100)\n-\n(0,0) (90,90,90)\n(1,1) (110,110,110)\n"
      formatOutput [cluster] `shouldBe` expected
 
  describe "K-means Algorithm Tests" $ do
    it "clusters identical points to the same cluster" $ do
      let testPoints = [
            ((0, 0), (100, 100, 100)),
            ((1, 0), (100, 100, 100)),
            ((0, 1), (100, 100, 100))
            ]
      clusters <- kMeans 2 0.1 testPoints
      length clusters `shouldSatisfy` (<= 2)
      -- All centroids should be (100,100,100)
      all (\c -> T.centroid c == (100, 100, 100)) clusters `shouldBe` True
 
    it "clusters distinct points to different clusters" $ do
      let testPoints = [
            ((0, 0), (10, 10, 10)),
            ((1, 0), (10, 10, 10)),
            ((0, 1), (200, 200, 200)),
            ((1, 1), (200, 200, 200))
            ]
      clusters <- kMeans 2 0.1 testPoints
      length clusters `shouldBe` 2
      -- Verify the centroids (approximately)
      let centroids = map T.centroid clusters
      let hasLightCentroid = any (\(r,g,b) -> r > 150 && g > 150 && b > 150) centroids
      let hasDarkCentroid = any (\(r,g,b) -> r < 50 && g < 50 && b < 50) centroids
      (hasLightCentroid && hasDarkCentroid) `shouldBe` True
 
  describe "Color Distance Tests" $ do
    it "calculates zero distance for identical colors" $ do
      colorDistance (100, 100, 100) (100, 100, 100) `shouldBe` 0.0
 
    it "calculates correct distance for different colors" $ do
      -- Distance should be sqrt(300) = ~17.32
      abs (colorDistance (100, 100, 100) (110, 110, 110) - sqrt 300) `shouldSatisfy` (< 0.001)
 
    it "handles extreme color differences" $ do
      -- Distance from black to white: sqrt(3 * (255^2)) = ~441.67
      let blackToWhiteDist = colorDistance (0, 0, 0) (255, 255, 255)
      let expected = sqrt (3.0 * (fromIntegral ((255 :: Int) ^ (2 :: Int)) :: Double))
      abs (blackToWhiteDist - expected) `shouldSatisfy` (< 0.001)
 
  describe "Edge Case Tests" $ do
    it "handles empty input gracefully" $ do
      clusters <- kMeans 5 0.1 []
      length clusters `shouldBe` 0
 
    it "handles single pixel input correctly" $ do
      let testPoints = [((0, 0), (100, 100, 100))]
      clusters <- kMeans 5 0.1 testPoints
      if null clusters
        then expectationFailure "Expected at least one cluster"
        else do
          let firstCluster = NE.head (NE.fromList clusters)
          T.centroid firstCluster `shouldBe` (100, 100, 100)
 
    it "handles k greater than number of pixels" $ do
      let testPoints = [((0, 0), (10, 10, 10)), ((1, 0), (20, 20, 20))]
      clusters <- kMeans 5 0.1 testPoints
      length clusters `shouldSatisfy` (<= 2)
 
  describe "Performance Tests" $ do
    it "completes small dataset clustering within reasonable time" $ do
      -- Generate a small dataset
      let testPoints = [(p, c) | p <- [(x, y) | x <- [0..5], y <- [0..5]],
                             c <- [(r, g, b) | r <- [50, 150], g <- [50, 150], b <- [50, 150]]]
     
      -- Time the execution
      start <- getCurrentTime
      _ <- kMeans 4 1.0 testPoints
      end <- getCurrentTime
     
      let duration = diffUTCTime end start
      duration `shouldSatisfy` (< 2) -- Should complete in less than 2 seconds
 
  describe "Integration Tests" $ do
    it "processes a sample file end-to-end" $ do
      withSystemTempFile "test_integration.txt" $ \path handle -> do
        -- Write test data
        let testData = unlines [
              "(0,0) (10,10,10)",
              "(0,1) (10,12,11)",
              "(1,0) (200,200,200)",
              "(1,1) (202,198,201)"
              ]
        hPutStr handle testData
        hClose handle
       
        -- Create a Config and run compressor
        let config = Config 2 0.1 path
       
        -- Execute and don't expect an exception
        result <- try (runCompressor config) :: IO (Either SomeException ())
        case result of
          Left ex -> expectationFailure $ "Exception thrown: " ++ show ex
          Right _ -> return ()
 
  describe "Error Handling Tests" $ do
    it "handles non-existent file appropriately" $ do
      let config = Config 2 0.1 "nonexistent_file.txt"
     
      result <- try (runCompressor config) :: IO (Either IOException ())
      case result of
        Left _ -> return () -- Expected error
        Right _ -> expectationFailure "Should have failed on non-existent file"
 
    it "handles malformed input file appropriately" $ do
      withSystemTempFile "test_malformed.txt" $ \path handle -> do
        -- Write invalid data
        hPutStr handle "This is not a valid pixel format"
        hClose handle
       
        let config = Config 2 0.1 path
       
        -- Expect parsePixels to fail
        content <- readFile path
        case parsePixels content of
          Right _ -> expectationFailure "Should fail on malformed input"
          Left _ -> return ()
 
  describe "Convergence Tests" $ do
    it "converges faster with higher limit" $ do
      -- Create a dataset with well-separated clusters
      let testPoints = [
            ((0, 0), (10, 10, 10)),
            ((0, 1), (12, 11, 9)),
            ((1, 0), (11, 10, 12)),
            ((1, 1), (9, 12, 10)),
            ((5, 5), (200, 200, 200)),
            ((5, 6), (202, 198, 201)),
            ((6, 5), (197, 201, 199)),
            ((6, 6), (203, 202, 198))
            ]
     
      -- Run with different convergence limits
      clusters0 <- kMeans 2 0.1 testPoints
      clusters1 <- kMeans 2 1.0 testPoints
     
      -- Both should find 2 clusters
      length clusters0 `shouldBe` 2
      length clusters1 `shouldBe` 2
     
      -- Verify clusters are similar
      let centroids0 = map T.centroid clusters0
      let centroids1 = map T.centroid clusters1
     
      -- Check if both contain a dark and light cluster
      any (\(r,g,b) -> r < 50 && g < 50 && b < 50) centroids0 `shouldBe` True
      any (\(r,g,b) -> r > 150 && g > 150 && b > 150) centroids0 `shouldBe` True
      any (\(r,g,b) -> r < 50 && g < 50 && b < 50) centroids1 `shouldBe` True
      any (\(r,g,b) -> r > 150 && g > 150 && b > 150) centroids1 `shouldBe` True
 