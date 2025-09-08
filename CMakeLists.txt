# set(CMAKE_C_COMPILER "/opt/homebrew/Cellar/llvm/20.1.3/bin/clang")
# set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/llvm/20.1.3/bin/clang++")
cmake_minimum_required(VERSION 3.22)
project(proj_name VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic")


find_package(GTest REQUIRED CONFIG)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

include(GoogleTest)
