cmake_minimum_required (VERSION 3.8)

# 1. Đặt tên dự án
project ("Joinless")

# 2. Thiết lập chuẩn C++ 17 (như trong c_cpp_properties cũ của bạn)
set (CMAKE_CXX_STANDARD 17)

# 3. Khai báo thư mục chứa file .h (Thay thế cho -I trong tasks.json)
include_directories ("${CMAKE_SOURCE_DIR}/src/c++/include")

# 4. Tìm tất cả các file .cpp (Thay thế cho args: *.cpp trong tasks.json)
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/c++/src/*.cpp")

# ==============================================================================
# Build Target
# ==============================================================================
# Create executable
add_executable (main ${SOURCE_FILES})


configure_file(
    "${CMAKE_SOURCE_DIR}/src/c++/config.txt"
    "${CMAKE_BINARY_DIR}/src/c++/config.txt"
    COPYONLY
)
file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_BINARY_DIR}/data")