project(InheritanceChain LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    inheritancechainmain.cpp
    inheritancechain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
