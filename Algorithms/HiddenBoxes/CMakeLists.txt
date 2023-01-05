cmake_minimum_required(VERSION 3.5)

project(HiddenBoxes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Shared
    ../LexicographicSorting
)

add_executable(${PROJECT_NAME}
    hiddenboxesmain.cpp
    boxutils.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Shared)
