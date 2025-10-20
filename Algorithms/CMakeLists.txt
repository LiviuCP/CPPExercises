cmake_minimum_required(VERSION 3.5)

project(Algorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(ChessHorse)
add_subdirectory(HuffmanEncoding)
add_subdirectory(KruskalPrim)
add_subdirectory(MapColouring)
add_subdirectory(DataOrdering)
add_subdirectory(WordsCounting)
add_subdirectory(STL)
add_subdirectory(Regex)
add_subdirectory(Tree)
add_subdirectory(Other)

# Some compiling errors occurred with these projects which were caused by the specific implementation of the std::sort algorithm
# so it has been decided to exclude them from Windows
if (NOT WIN32)
    add_subdirectory(HiddenBoxes)
    add_subdirectory(LexicographicSorting)
endif()
