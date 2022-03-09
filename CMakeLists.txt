cmake_minimum_required(VERSION 3.5)

project(CPPExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Algorithms)
add_subdirectory(CPPConcepts)
add_subdirectory(DesignPatterns)
add_subdirectory(InterProcessCommunication)
