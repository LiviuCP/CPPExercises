project(HiddenBoxes LANGUAGES CXX)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    hiddenboxesmain.cpp
    boxutils.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
