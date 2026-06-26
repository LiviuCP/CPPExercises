cmake_minimum_required(VERSION 3.28)

# implementation of ChessHorse using C++2X modules
project(ChessHorseV2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include_directories(
    ../../External/Matrix/MatrixLib/Matrix
    ../../Utilities/UtilitiesLib
)

add_executable(${PROJECT_NAME} chesshorsev2main.cpp)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET all_application_modules
        TYPE CXX_MODULES
        FILES chesstablev2.ixx commandargumentsparserv2.ixx
    PRIVATE chesstablev2.cpp commandargumentsparserv2.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
