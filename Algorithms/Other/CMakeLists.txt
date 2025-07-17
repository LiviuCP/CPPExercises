cmake_minimum_required(VERSION 3.5)

project(OtherAlgorithms LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Shared
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(MatrixAlgorithmsTests tst_matrixalgorithmstests.cpp)
add_test(NAME MatrixAlgorithmsTests COMMAND MatrixAlgorithmsTests)

target_link_libraries(MatrixAlgorithmsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
