project(MapColouring LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    mapcolouringmain.cpp
    mapcolouringutils.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET mapcolouring_modules
        TYPE CXX_MODULES
        FILES mapcolouringutils.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
