cmake_minimum_required(VERSION 3.5)

project(NetworkSockets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ../../Shared
)

add_executable(Client
    clientmain.cpp
    client.cpp
)

add_executable(Server
    servermain.cpp
    server.cpp
)

add_executable(ConcurrentClients
    concurrentclientsmain.cpp
    client.cpp
)

add_executable(ConcurrentServer
    concurrentservermain.cpp
    server.cpp
)

if(UNIX AND NOT APPLE)
    target_link_libraries(Client PRIVATE pthread)
    target_link_libraries(Server PRIVATE pthread)
    target_link_libraries(ConcurrentClients PRIVATE pthread)
    target_link_libraries(ConcurrentServer PRIVATE pthread)
endif()
