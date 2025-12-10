# ==============================================================================
# Project Configuration
# ==============================================================================
cmake_minimum_required (VERSION 3.8)

# Set project name
project ("Joinless")

# Set C++17 standard (as specified in c_cpp_properties)
set (CMAKE_CXX_STANDARD 17)

# ==============================================================================
# Include Directories
# ==============================================================================
# Declare directory containing .h files (replaces -I in tasks.json)
include_directories ("${CMAKE_SOURCE_DIR}/src/c++/include")

# ==============================================================================
# Source Files
# ==============================================================================
# Find all .cpp files (replaces *.cpp args in tasks.json)
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/c++/src/*.cpp")

# ==============================================================================
# Build Target
# ==============================================================================
# Create executable
add_executable (main ${SOURCE_FILES})