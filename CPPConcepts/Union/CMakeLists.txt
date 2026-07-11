project(Union LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    unionmain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
