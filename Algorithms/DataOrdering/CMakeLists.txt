project(DataOrdering LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    dataorderingmain.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET dataordering_modules
        TYPE CXX_MODULES
        FILES
            dataorderingengine.cpp
            dataordering_io.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
