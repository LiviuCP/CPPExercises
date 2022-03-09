cmake_minimum_required(VERSION 3.5)

project(BinarySearchTrees LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} STATIC
    binarysearchtree.cpp
    redblacktree.cpp
    avltree.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE BINARYSEARCHTREES_LIBRARY)
