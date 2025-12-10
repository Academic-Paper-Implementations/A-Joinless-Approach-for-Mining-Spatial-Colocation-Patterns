# C++ Codebase Refactoring Plan

## 🎯 Executive Summary

This refactoring plan focuses exclusively on **readability, maintainability, and clarity** improvements for the C++ Joinless Co-location Pattern Mining project. 

**CRITICAL CONSTRAINTS:**
- ✅ **ZERO logic changes** - No renaming of variables, functions, classes, or files
- ✅ **ZERO functional modifications** - Code will compile and run exactly as before
- ✅ **ZERO algorithm changes** - No data structure or execution flow modifications
- ✅ **ZERO performance optimizations** - Not the goal of this refactoring

**PURPOSE:**
- Improve code documentation and inline comments
- Standardize formatting and spacing
- Enhance project documentation for new developers

---

## 1️⃣ Global Changes

### 1.1 Formatting Standards (All .cpp and .h files)

**Indentation:**
- Use **4 spaces** consistently (already mostly applied, will fix inconsistencies)
- Ensure consistent indentation for nested blocks, function parameters, and templates

**Vertical Whitespace:**
- Add blank lines to separate logical sections within functions:
  - Between variable declarations and logic
  - Between different processing steps
  - Before and after loops/conditionals (when it improves clarity)
- Keep one blank line between function definitions
- Keep one blank line after includes before code starts

**Line Length:**
- Break long function calls or declarations across multiple lines when they exceed readability
- Align multi-line parameters for better readability
- Current code is generally good, will fix only problematic cases

**Code Block Separation:**
- Use comments to mark clear "Step 1", "Step 2" style divisions in complex functions
- Separate init, processing, and return sections with blank lines

### 1.2 Documentation Standards

**Doxygen Comments (All .h files):**
- Add `/** ... */` style comments for all classes
- Document all public functions with:
  - `@brief` - Short description of what the function does
  - `@param` - Description for each parameter
  - `@return` - Description of return value
  - `@note` - Any important usage notes (optional)

**Inline Comments (All .cpp files):**
- Add `//` comments to explain complex logic blocks
- Comment on "why" decisions were made, not just "what" the code does
- Add step-by-step explanations for multi-step algorithms

**Comment Cleanup:**
- Remove commented-out dead code (unless marked as important reference)
- Keep Vietnamese comments that explain important context, but ensure code comments are in English

### 1.3 CMake and Config Files

**CMakeLists.txt:**
- Translate Vietnamese comments to English
- Add section headers for better organization
- Add brief explanatory comments for each configuration step

**config.txt:**
- Ensure consistent formatting
- Add more descriptive comments for each parameter

---

## 2️⃣ File-by-File Plan

### 2.1 Header Files (include/*.h)

#### `types.h`
**Current State:** Basic comments present, structure is clear
**Changes:**
- ✅ Add Doxygen documentation for `SpatialInstance` struct
- ✅ Add Doxygen documentation for `StarNeighborhood` struct
- ✅ Document type aliases with brief comments explaining their purpose
- ✅ Add file header comment explaining the module's purpose

#### `config.h`
**Current State:** Has some comments, mostly clear
**Changes:**
- ✅ Convert existing C-style block comments to Doxygen format
- ✅ Add Doxygen documentation for `AppConfig` struct
- ✅ Add Doxygen documentation for `ConfigLoader` class and its `load()` method
- ✅ Document each AppConfig member variable inline

#### `data_loader.h`
**Current State:** Has a C-style comment block
**Changes:**
- ✅ Convert C-style comment to Doxygen format
- ✅ Add Doxygen documentation for `DataLoader` class
- ✅ Document the `load_csv()` method with parameters and return value
- ✅ Add brief note about CSV format expectations

#### `spatial_index.h`
**Current State:** Has C-style comments and some inline docs
**Changes:**
- ✅ Convert C-style comment to Doxygen format
- ✅ Add Doxygen documentation for `SpatialIndex` class
- ✅ Document constructor with parameter explanation
- ✅ Document `findNeighborPair()` method details
- ✅ Document private `euclideanDist()` method

#### `neighborhood_mgr.h`
**Current State:** Has C-style comment
**Changes:**
- ✅ Convert C-style comment to Doxygen format
- ✅ Add Doxygen documentation for `NeighborhoodMgr` class
- ✅ Document `buildFromPairs()` method
- ✅ Document `getStarNeighborhood()` and `getAllStarNeighborhoods()` methods
- ✅ Document `areNeighbors()` method

#### `utils.h`
**Current State:** Has inline comments for functions
**Changes:**
- ✅ Convert inline comments to Doxygen format
- ✅ Add file header explaining utility functions
- ✅ Document all three helper functions with Doxygen style
- ✅ Add parameter and return value descriptions

#### `miner.h`
**Current State:** Minimal comments, complex class
**Changes:**
- ✅ Add Doxygen file header
- ✅ Document `ProgressCallback` type alias
- ✅ Add Doxygen class documentation for `JoinlessMiner`
- ✅ Document all private methods (`filterStarInstances`, `filterCliqueInstances`, `selectPrevColocations`)
- ✅ Document public `mineColocations()` method with detailed parameter descriptions
- ✅ Document `generateCandidates()` method

#### `csv.hpp`
**Current State:** Third-party library
**Changes:**
- ❌ No changes - external library, do not modify

---

### 2.2 Source Files (src/*.cpp)

#### `main.cpp`
**Current State:** Well-structured with step comments, has some Vietnamese comments
**Changes:**
- ✅ Add file header comment explaining the program entry point
- ✅ Translate Vietnamese comment on line 23 to English
- ✅ Add blank line between configuration loading and data loading sections
- ✅ Add comment explaining the progress callback lambda function
- ✅ Improve spacing around the debug output sections
- ✅ Add brief comment explaining what each major step (1-5) accomplishes

**Specific Changes:**
- Line 23: Translate "Truyền tham số d từ config" → "Pass distance parameter d from config"
- Line 67: Change "In kết quả" → "Print results"
- Add spacing between Steps 3-4-5 for better visual separation

#### `config.cpp`
**Current State:** Clean implementation, minimal comments
**Changes:**
- ✅ Add file header comment
- ✅ Add inline comment explaining the file parsing logic
- ✅ Add comment explaining the default value behavior when config file not found
- ✅ Add blank line separating the file reading loop from the parsing logic
- ✅ Comment the key-value parsing mechanism

#### `data_loader.cpp`
**Current State:** Very simple, minimal comments
**Changes:**
- ✅ Add file header comment
- ✅ Add inline comments explaining CSV column mapping
- ✅ Add comment about instance ID generation logic (concatenation of type + number)
- ✅ Add blank line between loop and return statement

#### `spatial_index.cpp`
**Current State:** Clean code, minimal comments
**Changes:**
- ✅ Add file header comment
- ✅ Add inline comment in `findNeighborPair()` explaining the nested loop logic
- ✅ Add comment explaining the distance comparison threshold check
- ✅ Add blank lines separating variable declarations from loop logic
- ✅ Add comment explaining brute-force approach (for transparency about O(n²))

#### `neighborhood_mgr.cpp`
**Current State:** Complex logic, minimal comments
**Changes:**
- ✅ Add file header comment
- ✅ Add inline comments explaining the `buildFromPairs()` algorithm:
  - Comment explaining the star neighborhood concept
  - Comment on finding existing star neighborhood vs creating new one
  - Comment on the lambda find operation
- ✅ Add blank lines separating search logic from update logic
- ✅ Remove unnecessary `return;` statement on line 24 (it's void function, optional cleanup)

**Wait, this is a logic change to remove return. Let me reconsider:**
- ❌ Do NOT remove `return;` - keeping everything exactly as is

**Revised:**
- Just add explanatory comments without removing any code

#### `utils.cpp`
**Current State:** Simple helper functions with basic comments
**Changes:**
- ✅ Add file header comment
- ✅ Enhance comments for each helper function explaining their purpose
- ✅ Add blank lines between function implementations
- ✅ Add comment explaining the `getInstanceByID()` fallback behavior (returns empty struct if not found)
- ✅ Add inline comment in `countInstancesByFeature()` explaining the ID substring extraction logic

#### `miner.cpp`
**Current State:** Very long file (450 lines), has many Vietnamese comments, complex logic
**Changes:**
- ✅ Add comprehensive file header explaining the algorithm
- ✅ **Translate ALL Vietnamese comments to English:**
  - Line 16: "Gán tham số vào biến thành viên để sử dụng trong các method khác"
  - Line 233: "Lọc candidate (Đoạn này OK)"
  - Line 243: "Duyệt qua các ngôi sao"
  - Line 246-252: Block comment about fixing const issue
  - Line 260-262: Comment about ColocationInstance storing const pointers
  - Line 293-296: Comment about optimization and pre-calculation
  - Line 306-309: Comment about multi-threading preparation
  - Line 312-323: Various comments in parallel processing section
  - Line 340: Comment "TRA CỨU NHANH"
  - Line 358-365: Merge results comments
  - Line 377-409: Many Vietnamese comments explaining the algorithm steps

- ✅ **Add vertical spacing in `mineColocations()`:**
  - Separate initialization from main loop
  - Add spacing between progress callback and algorithm steps

- ✅ **Add inline comments in `generateCandidates()`:**
  - Explain prefix-based joining strategy
  - Clarify Apriori pruning logic

- ✅ **Add inline comments in `filterStarInstances()`:**
  - Explain center type filtering
  - Clarify neighbor map construction

- ✅ **Add inline comments in `filterCliqueInstances()`:**
  - Already has extensive Vietnamese comments - translate to English
  - Keep the section structure (Step 1, 2, 3, 4) but translate

- ✅ **Add inline comments in `selectPrevColocations()`:**
  - Already has Vietnamese comments - translate to English
  - Keep the step structure

- ✅ **Improve spacing throughout:**
  - Add blank lines before major loop sections
  - Separate calculation steps from data aggregation

**Vietnamese → English Translation List:**
```
Line 16: "Gán tham số vào biến thành viên để sử dụng trong các method khác"
→ "Assign parameters to member variables for use in other methods"

Line 172: "Generate candidate"
→ Already in English, keep

Line 175: "Take prefix of k-1 first element"
→ Already in English, keep

Line 181: "Just join when the prefix is equal"
→ Already in English, improve to "Only join when prefixes match"

Line 186: "Generate new candidate"
→ Already in English, keep

Line 197: "APRIORI PRUNING"
→ Already in English, keep

Line 217: "Remove duplicate"
→ Already in English, keep

Line 233: "Lọc candidate (Đoạn này OK)"
→ "Filter candidates (this section is correct)"

Line 243: "Duyệt qua các ngôi sao"
→ "Iterate through star neighborhoods"

Lines 246-262: All comments about fixing const issues
→ Translate to English explanation

Line 293: "TỐI ƯU HÓA TRA CỨU (PRE-CALCULATION)"
→ "LOOKUP OPTIMIZATION (PRE-CALCULATION)"

Line 295-296: Comment explaining optimization
→ Translate to English

Line 306: "CHUẨN BỊ CHO ĐA LUỒNG"
→ "MULTI-THREADING PREPARATION"

Line 307: "Lấy số lượng luồng tối đa"
→ "Get maximum number of threads"

Line 309: Comment about race condition
→ Translate to English

Line 312: "XỬ LÝ SONG SONG"
→ "PARALLEL PROCESSING"

Line 316: Comment about thread_id
→ Already clear, keep

Line 318: "Trích xuất feature types của instance hiện tại để so sánh"
→ "Extract feature types from current instance for comparison"

Line 327: "Kiểm tra xem instance có chứa candidate pattern này không"
→ "Check if instance contains this candidate pattern"

Line 333: "Kiểm tra tất cả các tập con (k-1) của candidate"
→ "Check all (k-1) subsets of candidate"

Line 335: "Tạo subset bằng cách loại bỏ phần tử thứ k"
→ "Create subset by removing the k-th element"

Line 341: "TRA CỨU NHANH: Kiểm tra subset có tồn tại trong dữ liệu cũ không"
→ "FAST LOOKUP: Check if subset exists in previous data"

Line 342: "Thay vì loop qua prevInstances, ta chỉ cần find trong set đã tạo ở bước 1"
→ "Instead of looping through prevInstances, just find in the set created in step 1"

Line 350: "Ghi vào buffer riêng của luồng hiện tại (An toàn)"
→ "Write to current thread's private buffer (thread-safe)"

Line 352: Comment about breaking after finding candidate
→ Translate to English

Line 358: "GỘP KẾT QUẢ (MERGE)"
→ "MERGE RESULTS"

Line 359: "Gom dữ liệu từ các buffer con về vector kết quả chính"
→ "Aggregate data from thread buffers into main result vector"

Line 377: "BƯỚC 1: Cấu trúc dữ liệu để gom nhóm (Aggregation)"
→ "STEP 1: Data structure for aggregation"

Lines 378-380: Comments explaining data structure
→ Translate to English

Lines 382-386: Comment about initialization
→ Translate to English

Line 388: "BƯỚC 2: Duyệt Instance MỘT LẦN duy nhất (Single Pass)"
→ "STEP 2: Single pass through instances"

Line 389: Comment about complexity
→ Translate to English

Line 391: "2a. Trích xuất Pattern từ Instance"
→ "2a. Extract pattern from instance"

Line 392: Comment with example
→ Translate to English

Line 397: Comment about sorting for key matching
→ Translate to English

Line 400: "2b. Kiểm tra xem pattern này có nằm trong danh sách candidates quan tâm không"
→ "2b. Check if this pattern is in the candidates of interest"

Line 403: "2c. Cập nhật thống kê participating instances"
→ "2c. Update participating instances statistics"

Line 404: Comment explaining map structure
→ Already clear

Line 411: "BƯỚC 3: Tính toán tỷ lệ và lọc (Duyệt qua Candidates)"
→ "STEP 3: Calculate ratios and filter (iterate through candidates)"

Line 412: Comment about complexity
→ Translate to English

Line 420: Comment about empty participatingMap
→ Translate to English

Line 423: "Lấy tổng số lượng instance toàn cục của feature này"
→ "Get total global instance count for this feature"

Line 431: "Lấy số lượng instance tham gia vào pattern"
→ "Get count of instances participating in pattern"
```

---

### 2.3 Configuration Files

#### `CMakeLists.txt`
**Current State:** Has Vietnamese comments
**Changes:**
- ✅ Translate all Vietnamese comments to English:
  - Line 3: "Đặt tên dự án" → "Set project name"
  - Line 6: "Thiết lập chuẩn C++ 17" → "Set C++17 standard"
  - Line 9: "Khai báo thư mục chứa file .h (Thay thế cho -I trong tasks.json)" → "Declare include directory (replaces -I in tasks.json)"
  - Line 12: "Tìm tất cả các file .cpp (Thay thế cho args: *.cpp trong tasks.json)" → "Find all .cpp files (replaces *.cpp args in tasks.json)"
  - Line 15: "Tạo file chạy .exe" → "Create executable"

- ✅ Add section header comments for better organization
- ✅ Improve formatting consistency

#### `config.txt`
**Current State:** Clear and well-organized
**Changes:**
- ✅ Minimal changes - already well documented
- ✅ Consider adding brief explanation for the neighbor_distance value (why 160 for Las Vegas dataset)

---

### 2.4 Documentation Files

#### `README.md`
**Current State:** Good structure, but missing C++ build instructions
**Changes:**
- ✅ Add a new section: **"🔧 How to Build & Run (C++)"** after the Python section
- ✅ Include prerequisites:
  - MinGW installation
  - CMake installation
  - Visual Studio Code extensions (C/C++, CMake Tools)
- ✅ Add Build Instructions:
  - Step 1: Configure CMake
  - Step 2: Build project
  - Step 3: Run executable
  - Step 4: Configure paths in `config.txt`
- ✅ Add example command lines for Windows (PowerShell)
- ✅ Add section about project structure for C++ specifically
- ✅ Add note about OpenMP dependency (since `miner.cpp` uses `omp.h`)

**Proposed Section:**
```markdown
## 🔧 How to Build & Run (C++)

### Prerequisites
- **MinGW**: MinGW-w64 GCC compiler
- **CMake**: Version 3.8 or higher
- **Visual Studio Code Extensions**:
  - C/C++ (Microsoft)
  - CMake Tools
- **OpenMP**: Included with MinGW (for parallel processing)

### Build Instructions

#### 1. Configure CMake
Open the project in VS Code, then:
```bash
# From the project root directory
cmake -S . -B out/build -G "MinGW Makefiles"
```

#### 2. Build the Project
```bash
cmake --build out/build
```

#### 3. Configure Paths
Edit `src/c++/config.txt` to set:
- `dataset_path`: Path to your CSV dataset (e.g., `data/LasVegas_x_y_alphabet_version_03_2.csv`)
- `neighbor_distance`: Distance threshold for spatial neighbors
- `min_prevalence`: Minimum prevalence threshold
- Other parameters as needed

#### 4. Run the Executable
```bash
# From project root
./out/build/main.exe src/c++/config.txt
```

### C++ Project Structure
```
src/c++/
├── include/           # Header files
│   ├── types.h        # Core data structures
│   ├── config.h       # Configuration loader
│   ├── data_loader.h  # CSV data loading
│   ├── spatial_index.h    # Spatial indexing
│   ├── neighborhood_mgr.h # Neighborhood management
│   ├── miner.h        # Joinless mining algorithm
│   └── utils.h        # Helper functions
├── src/               # Implementation files
│   ├── main.cpp       # Entry point
│   ├── config.cpp
│   ├── data_loader.cpp
│   ├── spatial_index.cpp
│   ├── neighborhood_mgr.cpp
│   ├── miner.cpp
│   └── utils.cpp
└── config.txt         # Runtime configuration
```

### Notes
- The C++ implementation uses **OpenMP** for parallel processing in the mining algorithm
- Make sure `config.txt` paths are relative to the project root when running the executable
```

#### `docs/src_C++_guide.md`
**Current State:** Very minimal, mostly Vietnamese, incomplete
**Changes:**
- ✅ Expand significantly to provide comprehensive C++ setup guide
- ✅ Translate Vietnamese content to English
- ✅ Add detailed step-by-step instructions for:
  - Installing MinGW on Windows
  - Installing CMake
  - Setting up VS Code
  - Configuring tasks.json and c_cpp_properties.json
- ✅ Add troubleshooting section
- ✅ Add section explaining the CSV parser library (csv.hpp)

**New Structure:**
```markdown
# C++ Development Guide

## Table of Contents
1. Prerequisites Installation
2. Setting Up VS Code
3. Building with CMake
4. Building with g++ directly
5. Troubleshooting
6. Dependencies

## 1. Prerequisites Installation

### Installing MinGW (Windows)
[Detailed instructions...]

### Installing CMake
[Detailed instructions...]

## 2. Setting Up VS Code

### Required Extensions
- C/C++ (Microsoft)
- CMake Tools
- (Optional) C++ Intellisense

### Configuration Files

#### tasks.json Setup
[Detailed instructions including the args configuration mentioned in original]

#### c_cpp_properties.json Setup
[Include paths setup]

## 3. Building with CMake
[Step-by-step from original README]

## 4. Building with g++ Directly
For quick compilation without CMake:
```bash
g++ src/c++/src/*.cpp \
    -I src/c++/include \
    -o main.exe \
    -std=c++17 \
    -fopenmp
```

## 5. Troubleshooting

### Common Issues
- Missing omp.h
- CSV parser not found
- Path issues in config.txt

## 6. Dependencies

### csv.hpp Library
- Source: https://github.com/vincentlaucsb/csv-parser
- Purpose: Parse CSV files efficiently
- Integration: Single header file in `include/csv.hpp`
```

#### `CONTRIBUTING.md`
**Current State:** Not reviewed yet, will check if relevant
**Changes:**
- ✅ Review and ensure C++ code style guidelines are mentioned
- ✅ Add section about documentation standards if not present

---

## 3️⃣ Documentation Plan

### 3.1 README.md Updates

**New Sections to Add:**
1. **C++ Build & Run Instructions** (detailed above)
2. **C++ Project Structure** (tree view of src/c++)
3. **Configuration Guide** (how to modify config.txt)
4. **Dependencies** (OpenMP, csv.hpp)

**Sections to Enhance:**
- Overview: Add mention of C++ implementation status
- Contributing: Reference the C++ build guide

### 3.2 Create/Enhance docs/src_C++_guide.md

**Transform from:** Minimal Vietnamese notes with links
**Transform to:** Comprehensive English guide covering:
- Installation (MinGW, CMake)
- IDE Setup (VS Code)
- Build Systems (CMake vs g++ direct)
- Configuration
- Troubleshooting
- Dependencies explanation

### 3.3 Inline Code Documentation

**Strategy:**
Focus on documenting the "why" and "what", not the "how":
- **Why**: Explain design decisions, algorithm choices, optimization rationale
- **What**: Explain what the function/class does at a high level
- **Not How**: The code itself shows how it works; comments explain context

**Example Transformation:**
```cpp
// Before (no comment):
for (size_t i = 0; i < prevPrevalent.size(); i++) {
    for (size_t j = i + 1; j < prevPrevalent.size(); j++) {
        Colocation prefix1(prevPrevalent[i].begin(), prevPrevalent[i].end() - 1);

// After (with context):
// Generate (k+1)-size candidates using Apriori-gen approach
// Only join pairs with matching (k-1) prefixes to reduce candidate space
for (size_t i = 0; i < prevPrevalent.size(); i++) {
    for (size_t j = i + 1; j < prevPrevalent.size(); j++) {
        // Extract k-1 prefix from each k-size prevalent pattern
        Colocation prefix1(prevPrevalent[i].begin(), prevPrevalent[i].end() - 1);
```

---

## 4️⃣ Summary of Changes by Category

### Documentation Files (2 files)
| File | Changes |
|------|---------|
| `README.md` | Add C++ build section, project structure, configuration guide |
| `docs/src_C++_guide.md` | Complete rewrite with comprehensive C++ setup instructions |

### Header Files (7 files)
| File | Changes |
|------|---------|
| `types.h` | Add Doxygen docs for structs and type aliases |
| `config.h` | Convert to Doxygen, document all members |
| `data_loader.h` | Convert to Doxygen, document class and methods |
| `spatial_index.h` | Convert to Doxygen, document class and methods |
| `neighborhood_mgr.h` | Convert to Doxygen, document class and methods |
| `utils.h` | Convert to Doxygen, document helper functions |
| `miner.h` | Add comprehensive Doxygen docs for complex mining class |

### Source Files (6 files)
| File | Changes |
|------|---------|
| `main.cpp` | Translate Vietnamese comments, add section comments, improve spacing |
| `config.cpp` | Add file header, inline comments explaining parsing logic |
| `data_loader.cpp` | Add file header, comments explaining CSV mapping |
| `spatial_index.cpp` | Add file header, comments explaining brute-force neighbor search |
| `neighborhood_mgr.cpp` | Add file header, detailed comments for star neighborhood building |
| `miner.cpp` | **Major**: Translate all Vietnamese comments to English, add comprehensive inline documentation |

### Build/Config Files (2 files)
| File | Changes |
|------|---------|
| `CMakeLists.txt` | Translate Vietnamese comments to English, add section headers |
| `config.txt` | Minimal - already well documented |

---

## 5️⃣ Verification Plan

Since this is a **formatting and documentation-only refactoring**, verification is straightforward:

### 5.1 Compilation Verification
**Objective**: Ensure code still compiles without errors or warnings

**Steps:**
```bash
# Clean build
rm -rf out/build
cmake -S . -B out/build -G "MinGW Makefiles"
cmake --build out/build

# Expected outcome: Successful build with no errors
```

### 5.2 Execution Verification
**Objective**: Ensure program runs exactly as before

**Steps:**
```bash
# Run with existing config
./out/build/main.exe src/c++/config.txt

# Expected outcome: 
# - Program executes successfully
# - Same output as before refactoring (neighbor pairs, colocations, debug messages)
# - No runtime errors
```

### 5.3 Behavior Verification
**Objective**: Ensure identical runtime behavior

**Approach:**
1. Before refactoring: Run program, capture output to `output_before.txt`
2. After refactoring: Run program, capture output to `output_after.txt`
3. Compare: `diff output_before.txt output_after.txt`
4. Expected: No differences (or only timestamp differences if any)

**Commands:**
```bash
# Before refactoring
./out/build/main.exe src/c++/config.txt > output_before.txt 2>&1

# After refactoring
./out/build/main.exe src/c++/config.txt > output_after.txt 2>&1

# Compare
diff output_before.txt output_after.txt
```

### 5.4 Code Review Verification
**Objective**: Human review to verify constraints were followed

**Checklist:**
- [ ] No variable/function/class/file renaming occurred
- [ ] No algorithm logic changes
- [ ] No data structure modifications
- [ ] All Vietnamese comments translated to English
- [ ] Doxygen comments added to all public APIs in headers
- [ ] Inline comments added to complex logic in source files
- [ ] Formatting is consistent (indentation, spacing)
- [ ] README.md has C++ build instructions
- [ ] docs/src_C++_guide.md is comprehensive

### 5.5 Documentation Verification
**Objective**: Ensure README is complete and accurate

**Manual Steps:**
1. Follow README build instructions from scratch (pretend you're a new developer)
2. Verify all commands work
3. Verify config.txt paths are explained
4. Verify prerequisites are listed

---

## 6️⃣ Notes and Considerations

### Vietnamese → English Translation
The codebase has significant Vietnamese comments, especially in `miner.cpp`. All will be professionally translated to English while preserving the original meaning and context.

### Doxygen Standard
We'll use standard Doxygen tags:
- `@brief` - Short description
- `@param` - Parameter description
- `@return` - Return value description
- `@note` - Additional notes
- `@see` - Cross-references

### File Headers
Each file will get a brief header comment like:
```cpp
/**
 * @file filename.cpp
 * @brief Brief description of what this file contains
 */
```

### OpenMP Dependency
`miner.cpp` uses `#include <omp.h>` for parallel processing. This needs to be documented in README and build instructions must include `-fopenmp` flag.

### No Breaking Changes
- No header guard style changes
- No #include reordering
- No namespace additions
- No const-correctness fixes (would be logic changes)
- No variable naming improvements

---

## 7️⃣ Estimated Impact

| Metric | Before | After | Change |
|--------|--------|-------|--------|
| Lines with comments | ~100 | ~300 | +200 |
| Doxygen-documented functions | 0 | ~30 | +30 |
| English comments | ~30% | 100% | +70% |
| Blank lines (for readability) | ~50 | ~100 | +50 |
| Documentation pages | 1.5 | 2.5 | +1 |
| Build complexity | Same | Same | 0 |
| Runtime performance | Baseline | Baseline | 0 |

---

## 8️⃣ Timeline Estimate

| Phase | Estimated Effort | Files Affected |
|-------|-----------------|----------------|
| Header file documentation | 2-3 hours | 7 files |
| Source file documentation + translation | 4-5 hours | 6 files |
| Build file cleanup | 30 minutes | 2 files |
| README enhancement | 1-2 hours | 1 file |
| C++ guide rewrite | 2-3 hours | 1 file |
| Testing & verification | 1-2 hours | All files |
| **Total** | **11-16 hours** | **17 files** |

---

## ✅ Approval Request

This plan is ready for your review. Please approve or request modifications before implementation begins.

**Key Points for Review:**
1. Does the documentation strategy meet your expectations?
2. Is the level of commenting appropriate (not too verbose, not too sparse)?
3. Are there specific sections in `miner.cpp` that need extra attention?
4. Should we prioritize English translations or inline explanations?
5. Any additional files to include in the refactoring scope?

---

**End of Plan**
