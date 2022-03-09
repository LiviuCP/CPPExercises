cmake_minimum_required(VERSION 3.5)

project(CPPConcepts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(ArrayReference)
add_subdirectory(EarlyAndLateBinding)
add_subdirectory(InheritanceChain)
add_subdirectory(PartialTemplateSpecialization)
add_subdirectory(Union)
add_subdirectory(VirtualDestructor)
