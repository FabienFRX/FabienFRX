# 🎨 Image Compressor
```ascii
  _____                                _____                                                   
 |_   _|                              / ____|                                                  
   | |  _ __ ___   __ _  __ _  ___   | |     ___  _ __ ___  _ __  _ __ ___  ___ ___  ___  _ __ 
   | | | '_ ` _ \ / _` |/ _` |/ _ \  | |    / _ \| '_ ` _ \| '_ \| '__/ _ \/ __/ __|/ _ \| '__|
  _| |_| | | | | | (_| | (_| |  __/  | |___| (_) | | | | | | |_) | | |  __/\__ \__ \ (_) | |   
 |_____|_| |_| |_|\__,_|\__, |\___|   \_____\___/|_| |_| |_| .__/|_|  \___||___/___/\___/|_|   
                         __/ |                             | |                                 
                        |___/                              |_|                                 
```

## 🔍 Overview

This project implements a basic image compression technique by reducing the number of colors in an image. The compression is performed in three conceptual steps:

1. Reading the image and extracting the color of each pixel
2. Clustering these colors using K-means, and replacing each color with the mean color of its cluster
3. Creating the compressed image with the reduced color palette

The main focus of this implementation is the second step: **the clustering of colors using the K-means algorithm**.

## ✨ Features

- Parallel implementation of K-means clustering for improved performance
- Command-line interface with options for customization
- Support for configurable number of colors in the final image
- Adjustable convergence limit for the K-means algorithm

## 🧮 Algorithm

The K-means algorithm clusters data points by:

1. Initializing K random centroids
2. Assigning each data point to the nearest centroid
3. Recalculating centroids based on the mean of points in each cluster
4. Repeating steps 2-3 until convergence

In this implementation, we use the Euclidean distance in RGB space:

```
d = √[(r₁-r₂)² + (g₁-g₂)² + (b₁-b₂)²]
```

## 📋 Requirements

- [Haskell](https://www.haskell.org/ghcup/)
- [Stack](https://docs.haskellstack.org/en/stable/README/)

## 🔨 Build

To build the project:

```bash
make
```

This will use Stack to build the project and create the executable.

## 📖 Usage

```
USAGE: ./imageCompressor -n N -l L -f F

    N    number of colors in the final image
    L    convergence limit
    F    path to the file containing the colors of the pixels
```

### 🔧 Parameters

- `-n N`: Number of colors (clusters) in the compressed image
- `-l L`: Convergence limit (algorithm stops when all centroids move less than this value)
- `-f F`: Path to the input file

### 📥 Input Format

The input file should contain pixel data in the following format:

```
(x,y) (r,g,b)
```

Where:
- `(x,y)` is the position of the pixel
- `(r,g,b)` is the color of the pixel with values between 0 and 255

Example:
```
(0,0) (33,18,109)
(0,1) (33,18,109)
(0,2) (33,21,109)
```

### 📤 Output Format

The program outputs the clustered colors in the following format:

```
--
(77,63,204)
-
(0,1) (98,99,233)
(2,0) (88,77,211)
...
--
(35,36,45)
-
(1,0) (33,16,94)
...
```

Each cluster is represented by:
- A header containing the mean color of the cluster
- A list of pixels assigned to that cluster

## 📁 Project Structure

- `src/Main.hs`: Entry point of the application
- `src/CLI.hs`: Command-line interface handling
- `src/Types.hs`: Type definitions for the project
- `src/Parser.hs`: Input file parsing
- `src/KMeans.hs`: Implementation of the K-means algorithm
- `src/Output.hs`: Formatting the output
- `src/Lib.hs`: Core application logic

## 🧪 Testing

To run tests:

```bash
stack test
```

## 🚀 Potential Enhancements

- Direct input/output from/to common image formats (PNG, JPEG, etc.)
- Real-time visualization of the clustering process
- Application of the K-means algorithm to other kinds of datasets
- Performance optimizations through more advanced parallel processing

## 👨‍💻 Authors

- [Gobijan Nathakrishnan](https://github.com/GobyNathan)
- [Fabien Fraixanet](https://github.com/FabienFRX)

## 📄 License

This project is licensed under the MIT License.