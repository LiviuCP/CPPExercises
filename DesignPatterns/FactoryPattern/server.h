#ifndef SERVER_H
#define SERVER_H

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

#endif // SERVER_H
