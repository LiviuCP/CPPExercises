cmake_minimum_required(VERSION 3.5)

project(BinarySearchTreesLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    binarysearchtreeslib.cpp
    binarysearchtree.cpp
    redblacktree.cpp
    avltree.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE BINARYSEARCHTREESLIB_LIBRARY)
