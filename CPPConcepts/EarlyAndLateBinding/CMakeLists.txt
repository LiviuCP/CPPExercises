cmake_minimum_required(VERSION 3.5)

project(EarlyAndLateBinding LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)

add_executable(${PROJECT_NAME}
    earlyandlatebindingmain.cpp
)
