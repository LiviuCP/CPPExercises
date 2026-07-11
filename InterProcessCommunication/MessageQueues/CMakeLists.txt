project(MessageQueues LANGUAGES CXX)

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

target_link_libraries(QueueSender PRIVATE UtilitiesLib)
target_link_libraries(QueueReceiver PRIVATE UtilitiesLib)
