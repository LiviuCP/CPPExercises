project(InterProcessCommunication LANGUAGES CXX)

if(UNIX)
    add_subdirectory(MessageQueues)
    add_subdirectory(NetworkSockets)
endif()

add_subdirectory(Semaphore)
add_subdirectory(Hosts)
add_subdirectory(CharCounter)
