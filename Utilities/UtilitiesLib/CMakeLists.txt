project(UtilitiesLib LANGUAGES CXX)

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET utilities_modules
        TYPE CXX_MODULES
        FILES datautils.ixx matrixutils.cpp utils.cpp
    PRIVATE datautils.cpp

)

target_link_libraries(${PROJECT_NAME} PUBLIC MatrixLib)
