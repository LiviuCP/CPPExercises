project(AdapterPattern LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    adapterpatternmain.cpp
    hdmidisplay.cpp
    hdmicard.cpp
    vgadisplay.cpp
    vgaadapter.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
