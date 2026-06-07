project(TreeTests LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

include_directories(
    ../TreeLib
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

add_executable(TreeTests tst_treetests.cpp)
add_test(NAME TreeTests COMMAND TreeTests)

target_link_libraries(TreeTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(TreeTests PRIVATE TreeLib)
