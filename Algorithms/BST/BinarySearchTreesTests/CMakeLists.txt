cmake_minimum_required(VERSION 3.5)

project(BinarySearchTreesTests LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../BinarySearchTreesLib)

enable_testing()

add_executable(SimpleBSTTests tst_simplebsttests.cpp testutils.cpp)
add_test(NAME SimpleBSTTests COMMAND SimpleBSTTests)

add_executable(RedBlackTreesTests tst_redblacktreestests.cpp testutils.cpp)
add_test(NAME RedBlackTreesTests COMMAND RedBlackTreesTests)

add_executable(AVLTreesTests tst_avltreestests.cpp testutils.cpp)
add_test(NAME AVLTreesTests COMMAND AVLTreesTests)

add_executable(MixedTreeTypesTests tst_mixedtreetypestests.cpp testutils.cpp)
add_test(NAME MixedTreeTypesTests COMMAND MixedTreeTypesTests)

target_link_libraries(SimpleBSTTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(SimpleBSTTests PRIVATE BinarySearchTreesLib)

target_link_libraries(RedBlackTreesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(RedBlackTreesTests PRIVATE BinarySearchTreesLib)

target_link_libraries(AVLTreesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(AVLTreesTests PRIVATE BinarySearchTreesLib)

target_link_libraries(MixedTreeTypesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(MixedTreeTypesTests PRIVATE BinarySearchTreesLib)
