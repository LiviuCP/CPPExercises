project(Regex LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

add_executable(RegexTests tst_regextests.cpp)
add_test(NAME RegexTests COMMAND RegexTests)

target_link_libraries(RegexTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
