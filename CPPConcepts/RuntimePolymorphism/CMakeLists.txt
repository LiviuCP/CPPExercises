project(RuntimePolymorphism LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    runtimepolymorphismmain.cpp
    containers.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET runtimepolymorphism_modules
        TYPE CXX_MODULES
        FILES datatypes.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
