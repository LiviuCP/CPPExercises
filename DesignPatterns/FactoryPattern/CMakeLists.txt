project(FactoryPattern LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    factorypatternmain.cpp
    server.cpp
    servertypes.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
