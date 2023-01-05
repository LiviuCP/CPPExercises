cmake_minimum_required(VERSION 3.5)

project(BST LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(BinarySearchTreesApp)
add_subdirectory(BinarySearchTreesLib)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test QUIET)

if (QT_FOUND)
    add_subdirectory(BinarySearchTreesTests)
endif()
