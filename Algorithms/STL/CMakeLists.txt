cmake_minimum_required(VERSION 3.5)

project(STLAlgorithms LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(SortingTests tst_sortingtests.cpp)
add_test(NAME SortingTests COMMAND SortingTests)

add_executable(PartitioningTests tst_partitioningtests.cpp)
add_test(NAME PartitioningTests COMMAND PartitioningTests)

add_executable(NonModifyingSequenceTests tst_nonmodifyingsequencetests.cpp)
add_test(NAME NonModifyingSequenceTests COMMAND NonModifyingSequenceTests)

target_link_libraries(SortingTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(PartitioningTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(NonModifyingSequenceTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
