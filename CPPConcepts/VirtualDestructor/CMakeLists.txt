project(VirtualDestructor LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    virtualdestructormain.cpp
    virtualdestructor.cpp
    nonvirtualdestructor.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
