cmake_minimum_required(VERSION 3.5)

project(NewerCPPConcepts LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(CPP17ConceptsTests tst_cpp17conceptstests.cpp)
add_test(NAME CPP17ConceptsTests COMMAND CPP17ConceptsTests)

add_executable(CPP20ConceptsTests tst_cpp20conceptstests.cpp)
add_test(NAME CPP20ConceptsTests COMMAND CPP17ConceptsTests)

target_link_libraries(CPP17ConceptsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(CPP20ConceptsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
