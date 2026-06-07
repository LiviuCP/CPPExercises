project(InheritanceChain LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    inheritancechainmain.cpp
    inheritancechain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
