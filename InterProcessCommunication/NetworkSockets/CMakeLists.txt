cmake_minimum_required(VERSION 3.5)

project(NetworkSockets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    Clients
    Servers
    ../../Shared
)

add_executable(Client
    Clients/clientmain.cpp
    Clients/client.cpp
)

add_executable(Server
    Servers/servermain.cpp
    Servers/server.cpp
)

add_executable(ConcurrentClients
    Clients/concurrentclientsmain.cpp
    Clients/client.cpp
)

add_executable(ConcurrentServer
    Servers/concurrentservermain.cpp
    Servers/server.cpp
)

target_link_libraries(Client PRIVATE Shared)
target_link_libraries(Server PRIVATE Shared)
target_link_libraries(ConcurrentClients PRIVATE Shared)
target_link_libraries(ConcurrentServer PRIVATE Shared)

if(UNIX AND NOT APPLE)
    target_link_libraries(Client PRIVATE pthread)
    target_link_libraries(Server PRIVATE pthread)
    target_link_libraries(ConcurrentClients PRIVATE pthread)
    target_link_libraries(ConcurrentServer PRIVATE pthread)
endif()
