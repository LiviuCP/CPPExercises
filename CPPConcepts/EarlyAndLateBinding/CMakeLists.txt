project(EarlyAndLateBinding LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    earlyandlatebindingmain.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
