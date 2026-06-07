project(UtilitiesLib LANGUAGES CXX)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    utilities.cpp
    utils.cpp
    datautils.cpp
    matrixutils.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE UTITILIES_LIBRARY)
