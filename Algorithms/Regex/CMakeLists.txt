cmake_minimum_required(VERSION 3.5)

project(Regex LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(RegexTests tst_regextests.cpp)
add_test(NAME RegexTests COMMAND RegexTests)

target_link_libraries(RegexTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
