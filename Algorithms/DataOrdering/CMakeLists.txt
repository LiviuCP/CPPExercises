project(DataOrdering LANGUAGES CXX)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    dataorderingmain.cpp
    dataorderingengine.cpp
    dataordering_io.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
