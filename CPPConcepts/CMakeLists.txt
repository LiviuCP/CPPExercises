project(CPPConcepts LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test QUIET)

# excluded from Windows due to some MOC file issues
if (QT_FOUND AND NOT WIN32)
    add_subdirectory(PartialTemplateSpecialization)
endif()

# CPP2X modules have only been tested for the CPPExercises project on Linux
if (NOT WIN32 AND NOT APPLE)
    add_subdirectory(ModulePartitions)
endif()

add_subdirectory(ArrayReference)
add_subdirectory(EarlyAndLateBinding)
add_subdirectory(InheritanceChain)
add_subdirectory(RuntimePolymorphism)
add_subdirectory(Union)
add_subdirectory(VirtualDestructor)
add_subdirectory(NewerConcepts)
add_subdirectory(CPPCasts)
