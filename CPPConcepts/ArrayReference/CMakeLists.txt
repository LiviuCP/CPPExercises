project(ArrayReference LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    arrayreferencemain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
