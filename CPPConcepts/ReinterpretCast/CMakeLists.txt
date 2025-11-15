cmake_minimum_required(VERSION 3.5)

project(ReinterpretCast LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    reinterpretcastmain.cpp
    objects.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
