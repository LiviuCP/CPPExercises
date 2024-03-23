cmake_minimum_required(VERSION 3.5)

project(InterProcessCommunication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX)
    add_subdirectory(MessageQueues)
    add_subdirectory(NetworkSockets)
endif()

add_subdirectory(Semaphore)
add_subdirectory(Hosts)
