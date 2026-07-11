project(HuffmanEncoding LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    huffmanmain.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET huffman_modules
        TYPE CXX_MODULES
        FILES huffmanencoder.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
