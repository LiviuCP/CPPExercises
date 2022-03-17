cmake_minimum_required(VERSION 3.5)

project(Timer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)

add_executable(${PROJECT_NAME}
    timermain.cpp
    timer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Shared)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
