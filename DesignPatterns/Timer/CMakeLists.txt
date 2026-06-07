project(Timer LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    timermain.cpp
    timer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
