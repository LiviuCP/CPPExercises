cmake_minimum_required(VERSION 3.5)

project(CharCounter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} charcountermain.cpp parser.cpp concreteparsers.cpp parserfactory.cpp parsingqueue.cpp parsingengine.cpp concreteaggregators.cpp aggregatorfactory.cpp)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
