cmake_minimum_required(VERSION 3.5)

project(CPPCasts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(ReinterpretCast
    reinterpretcastmain.cpp
    objects.cpp
    objectprintutils.cpp
)

add_executable(ConstCast
    constcastmain.cpp
    objects.cpp
    objectprintutils.cpp
)

target_link_libraries(ConstCast PRIVATE UtilitiesLib)
target_link_libraries(ReinterpretCast PRIVATE UtilitiesLib)
