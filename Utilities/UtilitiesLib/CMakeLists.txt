project(UtilitiesLib LANGUAGES CXX)

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET utils_modules
        TYPE CXX_MODULES
        FILES datautils.cpp matrixutils.cpp utils.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC MatrixLib)
