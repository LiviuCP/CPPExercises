project(KruskalPrim LANGUAGES CXX)

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
