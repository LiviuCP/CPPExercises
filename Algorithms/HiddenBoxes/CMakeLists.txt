project(HiddenBoxes LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    hiddenboxesmain.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET hiddenboxes_modules
        TYPE CXX_MODULES
        FILES boxutils.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
