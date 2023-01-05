cmake_minimum_required(VERSION 3.5)

project(Union LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)

add_executable(${PROJECT_NAME}
    unionmain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Shared)
