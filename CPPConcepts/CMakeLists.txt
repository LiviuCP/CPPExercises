cmake_minimum_required(VERSION 3.5)

project(CPPConcepts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test QUIET)

# excluded from Windows due to some MOC file issues
if (QT_FOUND AND NOT WIN32)
    add_subdirectory(PartialTemplateSpecialization)
endif()

add_subdirectory(ArrayReference)
add_subdirectory(EarlyAndLateBinding)
add_subdirectory(InheritanceChain)
add_subdirectory(RuntimePolymorphism)
add_subdirectory(Union)
add_subdirectory(VirtualDestructor)
add_subdirectory(NewerConcepts)
add_subdirectory(ReinterpretCast)
