project(Accumulator LANGUAGES CXX)

add_executable(${PROJECT_NAME} accumulatormain.cpp)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET all_application_modules
        TYPE CXX_MODULES
        FILES
            Module/accumulator.cpp
            Module/accumulator_header.cpp
            Module/accumulator_impl.cpp
)
