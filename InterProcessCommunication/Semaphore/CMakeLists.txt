project(Semaphore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) # kept C++17 here due to an error encountered on Linux caused by the implementation of condition_variable

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    semaphoremain.cpp
    semaphore.cpp
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
