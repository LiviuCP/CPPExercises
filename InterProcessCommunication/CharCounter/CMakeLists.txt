project(CharCounter LANGUAGES CXX)

add_executable(${PROJECT_NAME} charcountermain.cpp parser.cpp concreteparsers.cpp parserfactory.cpp parsingqueue.cpp parsingengine.cpp concreteaggregators.cpp aggregatorfactory.cpp)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
