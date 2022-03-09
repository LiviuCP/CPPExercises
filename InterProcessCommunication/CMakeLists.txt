cmake_minimum_required(VERSION 3.5)

project(InterProcessCommunication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(MessageQueues)
add_subdirectory(NetworkSockets)
add_subdirectory(Semaphore)
