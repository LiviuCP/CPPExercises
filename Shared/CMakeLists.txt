cmake_minimum_required(VERSION 3.5)

project(Shared LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../External/Matrix/MatrixLib/Matrix
)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    shared.cpp
    utils.cpp
    datautils.cpp
    matrixutils.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SHARED_LIBRARY)
