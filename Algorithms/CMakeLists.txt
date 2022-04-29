cmake_minimum_required(VERSION 3.5)

project(Algorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(BST)
add_subdirectory(ChessHorse)
add_subdirectory(HuffmanEncoding)
add_subdirectory(KruskalTree)
add_subdirectory(MapColouring)
add_subdirectory(DataOrdering)

# Some compiling errors occurred with these projects which were caused by the specific implementation of the std::sort algorithm
# so it has been decided to exclude them from Windows
if (NOT WIN32)
    add_subdirectory(HiddenBoxes)
    add_subdirectory(LexicographicSorting)
endif()
