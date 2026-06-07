project(LexicographicSorting LANGUAGES CXX)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    lexicographicsortingmain.cpp
    lexicographicalsorter.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
