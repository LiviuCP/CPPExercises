project(Algorithms LANGUAGES CXX)

add_subdirectory(ChessHorse)

if (NOT WIN32 AND NOT APPLE)
    #newer version using modules that has only been tested on Linux
    add_subdirectory(ChessHorseV2)
endif()

add_subdirectory(HuffmanEncoding)
add_subdirectory(KruskalPrim)
add_subdirectory(MapColouring)
add_subdirectory(DataOrdering)
add_subdirectory(WordsCounting)
add_subdirectory(STL)
add_subdirectory(Regex)
add_subdirectory(Tree)

# Some compiling errors occurred with these projects which were caused by the specific implementation of the std::sort algorithm
# so it has been decided to exclude them from Windows
if (NOT WIN32)
    add_subdirectory(HiddenBoxes)
endif()
