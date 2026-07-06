project(UtilitiesTests LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

add_executable(DataUtilsTests tst_datautilstests.cpp)
add_test(NAME DataUtilsTests COMMAND DataUtilsTests)

add_executable(MatrixUtilsTests tst_matrixutilstests.cpp)
add_test(NAME MatrixUtilsTests COMMAND MatrixUtilsTests)

target_link_libraries(DataUtilsTests PRIVATE UtilitiesLib)
target_link_libraries(DataUtilsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_link_libraries(MatrixUtilsTests PRIVATE UtilitiesLib)
target_link_libraries(MatrixUtilsTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
