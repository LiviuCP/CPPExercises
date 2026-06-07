project(RuntimePolymorphism LANGUAGES CXX)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    runtimepolymorphismmain.cpp
    containers.cpp
    datatypes.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
