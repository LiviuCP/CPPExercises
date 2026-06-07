project(CPPCasts LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(ReinterpretCast
    reinterpretcastmain.cpp
    objects.cpp
    objectprintutils.cpp
)

add_executable(ConstCast
    constcastmain.cpp
    objects.cpp
    objectprintutils.cpp
)

target_link_libraries(ConstCast PRIVATE UtilitiesLib)
target_link_libraries(ReinterpretCast PRIVATE UtilitiesLib)
