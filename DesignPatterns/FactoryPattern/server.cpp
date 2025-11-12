#include "server.h"
#include "servertypes.h"

Server* Server::createServer(Server::ServerTypes serverType)
{
    Server* result{nullptr};

    switch (serverType)
    {
    case Server::ServerTypes::WEB:
        result = new WebServer;
        break;
    case Server::ServerTypes::MAIL:
        result = new MailServer;
        break;
    case Server::ServerTypes::FTP:
        result = new FTPServer;
        break;
    case Server::ServerTypes::FIREWALL:
        result = new Firewall;
        break;
    default:
        break;
    }

    return result;
}

Server::~Server()
{
}
