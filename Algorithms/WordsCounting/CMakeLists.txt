project(WordsCounting LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    wordscountingmain.cpp wordscounter.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
