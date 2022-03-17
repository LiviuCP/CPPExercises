cmake_minimum_required(VERSION 3.5)

project(CPPExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# this variable is propagated to all nested libraries (should be declared BEFORE any subdir)
set(LIB_TYPE SHARED)

# errors were encountered when building some libraries as shared on Windows so it has been decided to keep it static on this OS for the moment
if(WIN32)
    set(LIB_TYPE STATIC)
endif()

add_subdirectory(Algorithms)
add_subdirectory(CPPConcepts)
add_subdirectory(DesignPatterns)
add_subdirectory(InterProcessCommunication)
add_subdirectory(Shared)
