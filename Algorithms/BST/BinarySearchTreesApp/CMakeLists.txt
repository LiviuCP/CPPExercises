cmake_minimum_required(VERSION 3.5)

project(BinarySearchTreesApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../../../Shared)
include_directories(../BinarySearchTreesLib)

add_executable(${PROJECT_NAME} binarysearchtreesappmain.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE BinarySearchTreesLib)
target_link_libraries(${PROJECT_NAME} PRIVATE Shared)
