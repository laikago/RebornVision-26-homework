cmake_minimum_required(VERSION 3.12)
project(RedEdgeDetector)

set(CMAKE_CXX_STANDARD 11)

# 查找OpenCV
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# 添加可执行文件
add_executable(RedEdgeDetector
    src/RedEdgeDetector.cpp
    main.cpp
)

# 链接OpenCV库
target_link_libraries(RedEdgeDetector ${OpenCV_LIBS})
