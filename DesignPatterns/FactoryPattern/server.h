#pragma once

#include <string>

class Server
{
public:
    enum class ServerTypes
    {
        WEB,
        MAIL,
        FTP,
        FIREWALL,
        ServerTypesCount
    };

    static Server* createServer(ServerTypes serverType);

    virtual std::string getDescription() const = 0;
    virtual ~Server();
};
