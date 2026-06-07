project(AdapterPattern LANGUAGES CXX)

include_directories(
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    adapterpatternmain.cpp
    hdmidisplay.cpp
    hdmicard.cpp
    vgadisplay.cpp
    vgaadapter.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
