cmake_minimum_required(VERSION 3.10)

project(MatrixLibraryDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加头文件目录
include_directories(include)

# 添加源文件
set(SOURCES
    src/matrix.cpp
    src/main.cpp
)

# 生成可执行文件
add_executable(matrix_demo ${SOURCES})    