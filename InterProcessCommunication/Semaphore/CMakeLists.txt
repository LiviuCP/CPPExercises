cmake_minimum_required(VERSION 3.5)

project(Semaphore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) # kept C++17 here due to an error encountered on Linux caused by the implementation of condition_variable
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)

add_executable(${PROJECT_NAME}
    semaphoremain.cpp
    semaphore.cpp
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Shared)
