project(ChessHorse LANGUAGES CXX)

add_executable(${PROJECT_NAME} chesshorsemain.cpp)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET all_application_modules
        TYPE CXX_MODULES
        FILES chesstable.ixx commandargumentsparser.ixx
    PRIVATE chesstable.cpp commandargumentsparser.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilitiesLib)
