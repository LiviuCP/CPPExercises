cmake_minimum_required(VERSION 3.5)

project(Hosts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Hosts hostsmain.cpp csvparsingqueue.cpp csvparser.cpp csvaggregator.cpp apputils.cpp)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

include(GNUInstallDirs)
install(TARGETS Hosts
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# copy examples into build dir
set(CSVFILES CSVFilesExamples)
set(SRC_INPUT_CSV_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/InputCSVFilesExamples)
set(DEST_INPUT_CSV_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/InputCSVFilesExamples)
set(SRC_OUTPUT_CSV_FILE ${CMAKE_CURRENT_SOURCE_DIR}/exampleoutput.csv)
set(DEST_OUTPUT_CSV_FILE ${CMAKE_CURRENT_BINARY_DIR}/exampleoutput.csv)

add_custom_target(${CSVFILES} ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_INPUT_CSV_FILES_DIR})

add_custom_command(TARGET ${CSVFILES} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SRC_INPUT_CSV_FILES_DIR} ${DEST_INPUT_CSV_FILES_DIR})

add_custom_command(TARGET ${CSVFILES} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${SRC_OUTPUT_CSV_FILE} ${DEST_OUTPUT_CSV_FILE})
