cmake_minimum_required(VERSION 3.5)

project(KruskalPrim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(Kruskal
    kruskalmain.cpp
    kruskal.cpp
    baseengine.cpp
    treeutils.cpp
)

add_executable(Prim
    primmain.cpp
    prim.cpp
    baseengine.cpp
    treeutils.cpp
)

target_link_libraries(Kruskal PRIVATE UtilitiesLib)
target_link_libraries(Prim PRIVATE UtilitiesLib)
