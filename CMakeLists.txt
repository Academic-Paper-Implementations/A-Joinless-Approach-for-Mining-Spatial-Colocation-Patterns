cmake_minimum_required (VERSION 3.8)

# ============================================================================
# Project Configuration
# ============================================================================

# Set project name
project ("Joinless")

# Set C++17 standard
set (CMAKE_CXX_STANDARD 17)

# ============================================================================
# Include Directories
# ============================================================================

# Declare include directory (replaces -I in tasks.json)
include_directories ("${CMAKE_SOURCE_DIR}/src/c++/include")

# ============================================================================
# Source Files
# ============================================================================

# Find all .cpp files (replaces *.cpp args in tasks.json)
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/c++/src/*.cpp")

# ============================================================================
# Build Target
# ============================================================================

# Create executable
add_executable (main ${SOURCE_FILES})