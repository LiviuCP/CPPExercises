cmake_minimum_required(VERSION 3.5)

project(MessageQueues LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)

add_executable(QueueSender
    queuesendermain.cpp
    queuesender.cpp
    queuedataobjects.cpp
    queuedatamessages.cpp
)

add_executable(QueueReceiver
    queuereceivermain.cpp
    queuereceiver.cpp
    queuedataobjects.cpp
    queuedatamessages.cpp
)

target_link_libraries(QueueSender PRIVATE Shared)
target_link_libraries(QueueReceiver PRIVATE Shared)
