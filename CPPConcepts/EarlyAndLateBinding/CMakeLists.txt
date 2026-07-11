project(EarlyAndLateBinding LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    earlyandlatebindingmain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
