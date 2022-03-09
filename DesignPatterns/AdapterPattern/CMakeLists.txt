cmake_minimum_required(VERSION 3.5)

project(AdapterPattern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)
add_executable(${PROJECT_NAME}
    adapterpatternmain.cpp
    hdmidisplay.cpp
    hdmicard.cpp
    vgadisplay.cpp
    vgaadapter.cpp
)
