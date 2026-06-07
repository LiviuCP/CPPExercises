project(WordsCounting LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    wordscountingmain.cpp wordscounter.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
