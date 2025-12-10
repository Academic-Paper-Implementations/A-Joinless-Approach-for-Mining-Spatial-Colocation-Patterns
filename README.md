# A Joinless Approach for Mining Spatial Colocation Patterns

## 📋 Overview
This project implements the **Joinless Co-location Pattern Mining** algorithm.
It is structured to support multiple implementations (Python, C++, Java) and keeps documentation and data separate.

## 📂 Project Structure

```
root/
├── data/                           # Shared datasets
├── docs/                           # Documentation & Papers
│   ├── joinless_algorithm.pdf      # Original Paper
│   ├── README_BUILD_DATASET.md     # Data structure docs
│   └── src_C++_guide.md            # C++ development guide
├── notebooks/                      # Jupyter Notebooks
│   └── analysis.ipynb              # Analysis & Visualization
├── src/                            # Source Code
│   ├── python/                     # Python Implementation
│   │   ├── joinless/               # Main Package
│   │   │   ├── structures.py       # Data structures
│   │   │   ├── data_loader.py      # Loading logic
│   │   │   └── mining.py           # Algorithm implementation
│   │   └── main.py                 # Entry point
│   ├── c++/                        # C++ Implementation
│   │   ├── include/                # Header files
│   │   │   ├── types.h             # Core data structures
│   │   │   ├── config.h            # Configuration loader
│   │   │   ├── data_loader.h       # CSV data loading
│   │   │   ├── spatial_index.h     # Spatial indexing
│   │   │   ├── neighborhood_mgr.h  # Neighborhood management
│   │   │   ├── miner.h             # Joinless mining algorithm
│   │   │   └── utils.h             # Helper functions
│   │   ├── src/                    # Implementation files
│   │   │   ├── main.cpp            # Entry point
│   │   │   ├── config.cpp
│   │   │   ├── data_loader.cpp
│   │   │   ├── spatial_index.cpp
│   │   │   ├── neighborhood_mgr.cpp
│   │   │   ├── miner.cpp
│   │   │   └── utils.cpp
│   │   └── config.txt              # Runtime configuration
│   └── java/                       # Java Implementation (Future)
├── CMakeLists.txt                  # CMake build configuration
└── requirements.txt                # Python dependencies
```

## 🚀 How to Run (Python)

### 1. Setup
Ensure you have the required packages:
```bash
pip install -r requirements.txt
```

### 2. Run the Main Script
You can run the python implementation directly:
```bash
python src/python/main.py
```

### 3. Use Notebooks
Open `notebooks/analysis.ipynb` in Jupyter.
*Note: You may need to adjust the path imports in the notebook to point to `../src/python`.*

## 🔧 How to Build & Run (C++)

### Prerequisites

Before building the C++ implementation, ensure you have the following installed:

- **MinGW-w64**: GCC compiler for Windows
  - Download from: https://www.mingw-w64.org/
  - Make sure `g++` is in your PATH
- **CMake**: Version 3.8 or higher
  - Download from: https://cmake.org/download/
  - Add CMake to your PATH during installation
- **Visual Studio Code** (recommended):
  - Extensions: C/C++ (Microsoft), CMake Tools
- **OpenMP**: Included with MinGW (used for parallel processing in the mining algorithm)

### Build Instructions

#### Method 1: Using CMake (Recommended)

1. **Configure the build**
   ```powershell
   # From the project root directory
   cmake -S . -B out/build -G "MinGW Makefiles"
   ```

2. **Build the project**
   ```powershell
   cmake --build out/build
   ```

3. **The executable will be created at**: `out/build/main.exe`

#### Method 2: Direct compilation with g++

For quick compilation without CMake:
```powershell
g++ src/c++/src/*.cpp `
    -I src/c++/include `
    -o main.exe `
    -std=c++17 `
    -fopenmp
```

### Configuration

Before running, configure the algorithm parameters in `src/c++/config.txt`:

```ini
# I/O Paths
dataset_path=data/LasVegas_x_y_alphabet_version_03_2.csv
output_path=results/colocation_rules.txt

# Algorithm Thresholds
neighbor_distance=160        # Distance threshold for spatial neighbors
min_prevalence=0.15          # Minimum prevalence threshold (0.0 to 1.0)
min_cond_prob=0.5            # Minimum conditional probability for rules

# Debug
debug_mode=true              # Enable detailed debug output
```

**Important Notes:**
- `dataset_path`: Path relative to project root
- `neighbor_distance`: Adjust based on your dataset's spatial scale
- `min_prevalence`: Lower values find more patterns but may be less significant

### Running the C++ Implementation

```powershell
# From project root
./out/build/main.exe src/c++/config.txt
```

Or if you used direct compilation:
```powershell
./main.exe src/c++/config.txt
```

### Expected Output

The program will display:
1. **Configuration loading** - Confirms settings from config.txt
2. **Data loading** - Number of spatial instances loaded
3. **Spatial indexing** - Number of neighbor pairs found
4. **Neighborhood materialization** - Star neighborhoods created
5. **Mining progress** - Real-time progress with percentage completion
6. **Results** - All prevalent colocation patterns found

Example output:
```
[DEBUG] Step 1: Loading configuration...
[DEBUG] Step 1: Configuration loaded successfully.
Running Joinless with d=160...
[DEBUG] Step 2: Loading data from data/LasVegas_x_y_alphabet_version_03_2.csv...
[DEBUG] Step 2: Loaded 5000 instances.
[DEBUG] Step 3: Building spatial index with d=160...
[DEBUG] Step 3: Found 12500 neighbor pairs.
[DEBUG] Step 4: Materializing neighborhoods...
[DEBUG] Step 4: Neighborhoods materialized.
[DEBUG] Step 5: Mining colocation patterns with minPrev=0.15...
[PROGRESS] 95.0% (3/3) - Found 15 prevalent k=3 colocations
[DEBUG] Step 5: Mining completed.

Found 42 prevalent colocations:
Colocation: A - B
Colocation: A - C
Colocation: B - C
Colocation: A - B - C
...
[DEBUG] All steps completed successfully.
```

### C++ Implementation Features

- **Parallel Processing**: Uses OpenMP for multi-threaded clique instance filtering
- **Efficient Data Structures**: Optimized hash maps and sets for fast lookups
- **Progress Reporting**: Real-time progress callbacks during mining
- **Configurable**: All parameters adjustable via config.txt
- **Well-Documented**: Comprehensive Doxygen-style documentation in headers

### Troubleshooting

**Issue**: `cmake: command not found`
- **Solution**: Add CMake to your PATH or use full path to cmake.exe

**Issue**: `g++: command not found`
- **Solution**: Install MinGW and add its bin directory to PATH

**Issue**: `omp.h: No such file or directory`
- **Solution**: Ensure you're using MinGW-w64 which includes OpenMP support

**Issue**: Program crashes or gives incorrect results
- **Solution**: Check that `config.txt` paths are correct and relative to project root

**Issue**: Very slow performance
- **Solution**: 
  - Reduce `neighbor_distance` to decrease neighbor pairs
  - Increase `min_prevalence` to prune more candidates
  - Ensure OpenMP is enabled (check for `-fopenmp` flag)

For more detailed C++ development information, see [`docs/src_C++_guide.md`](docs/src_C++_guide.md).

## 🤝 Contributing
* **Python**: Work in `src/python/joinless`.
* **C++**: Work in `src/c++` (see `docs/src_C++_guide.md` for setup).
* **Java**: Add your implementations in `src/java`.