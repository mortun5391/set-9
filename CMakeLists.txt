cmake_minimum_required(VERSION 3.16)
project(string_sort_research LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(string_sort_benchmark
    src/main.cpp
    src/StringGenerator.cpp
    src/StringSortAlgorithms.cpp
    src/StringSortTester.cpp
)

target_include_directories(string_sort_benchmark PRIVATE include)

target_compile_options(string_sort_benchmark PRIVATE -O2 -Wall -Wextra -Wpedantic)
