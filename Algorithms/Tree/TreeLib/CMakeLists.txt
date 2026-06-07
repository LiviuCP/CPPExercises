project(TreeLib LANGUAGES CXX)

add_library(${PROJECT_NAME} ${LIB_TYPE}
    treelib.cpp
    tree.cpp
    node.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE TREELIB_LIBRARY)
