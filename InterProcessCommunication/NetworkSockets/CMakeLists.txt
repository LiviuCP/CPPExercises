project(NetworkSockets LANGUAGES CXX)

include_directories(
    Clients
    Servers
    ../../Utilities/UtilitiesLib
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

target_link_libraries(Client PRIVATE UtilitiesLib)
target_link_libraries(Server PRIVATE UtilitiesLib)
target_link_libraries(ConcurrentClients PRIVATE UtilitiesLib)
target_link_libraries(ConcurrentServer PRIVATE UtilitiesLib)

if(UNIX AND NOT APPLE)
    target_link_libraries(Client PRIVATE pthread)
    target_link_libraries(Server PRIVATE pthread)
    target_link_libraries(ConcurrentClients PRIVATE pthread)
    target_link_libraries(ConcurrentServer PRIVATE pthread)
endif()
