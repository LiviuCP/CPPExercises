project(ChessHorse LANGUAGES CXX)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME}
    chesshorsemain.cpp
    chesstable.cpp
    commandargumentsparser.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
