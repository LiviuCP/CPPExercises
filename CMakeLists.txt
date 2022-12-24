cmake_minimum_required(VERSION 3.5)

project(CPPExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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

set(INPUT_OUTPUT_DIR /tmp/InputOutputFiles)

if(WIN32)
    set(INPUT_OUTPUT_DIR C:\\InputOutputFiles)
endif()

# copy_directory copies the content of the source dir (not the dir itself) so it should be ensured
# a destination dir has previously been created in order to avoid polluting /tmp or C: with multiple IO files

add_custom_target(${PROJECT_NAME} ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${INPUT_OUTPUT_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/InputFileExamples ${INPUT_OUTPUT_DIR})
