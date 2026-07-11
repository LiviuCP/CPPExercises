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

target_sources(Kruskal
    PUBLIC
        FILE_SET kruskal_modules
        TYPE CXX_MODULES
        FILES baseengine.cpp kruskal.cpp
)

add_executable(Prim
    primmain.cpp
    prim.cpp
    baseengine.cpp
    treeutils.cpp
)

target_sources(Prim
    PUBLIC
        FILE_SET prim_modules
        TYPE CXX_MODULES
        FILES baseengine.cpp prim.cpp
)

target_link_libraries(Kruskal PRIVATE UtilitiesLib)
target_link_libraries(Prim PRIVATE UtilitiesLib)
