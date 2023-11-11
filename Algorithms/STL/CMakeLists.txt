cmake_minimum_required(VERSION 3.5)

project(STLAlgorithms LANGUAGES CXX)

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

add_executable(NonModifyingSequenceTests tst_nonmodifyingsequencetests.cpp)
add_test(NAME NonModifyingSequenceTests COMMAND NonModifyingSequenceTests)

add_executable(ModifyingSequenceTests tst_modifyingsequencetests.cpp)
add_test(NAME ModifyingSequenceTests COMMAND ModifyingSequenceTests)

add_executable(PartitioningTests tst_partitioningtests.cpp)
add_test(NAME PartitioningTests COMMAND PartitioningTests)

add_executable(SortingTests tst_sortingtests.cpp)
add_test(NAME SortingTests COMMAND SortingTests)

add_executable(SortedRangesTests tst_sortedrangestests.cpp)
add_test(NAME SortedRangesTests COMMAND SortedRangesTests)

add_executable(HeapTests tst_heaptests.cpp)
add_test(NAME HeapTests COMMAND HeapTests)

add_executable(MinMaxTests tst_minmaxtests.cpp)
add_test(NAME MinMaxTests COMMAND MinMaxTests)

add_executable(ComparisonPermutationTests tst_comparisonpermutationtests.cpp)
add_test(NAME ComparisonPermutationTests COMMAND ComparisonPermutationTests)

add_executable(NumericOperationsTests tst_numericoperationstests.cpp)
add_test(NAME NumericOperationsTests COMMAND NumericOperationsTests)

add_executable(UninitializedMemoryOperationsTests tst_uninitializedmemoryoperationstests.cpp)
add_test(NAME UninitializedMemoryOperationsTests COMMAND UninitializedMemoryOperationsTests)

add_executable(CombinedSTLTests tst_combinedstltests.cpp)
add_test(NAME CombinedSTLTests COMMAND CombinedSTLTests)

target_link_libraries(NonModifyingSequenceTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(ModifyingSequenceTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(PartitioningTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(SortingTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(SortedRangesTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(HeapTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(MinMaxTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(ComparisonPermutationTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(NumericOperationsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(UninitializedMemoryOperationsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(CombinedSTLTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
