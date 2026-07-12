/* This app shows a basic implmentation of the (abstract) factory design pattern*/

#include <iostream>

#include "server.h"
#include "utils.h"

int main()
{
    Utilities::clearScreen();

    std::cout << "Let's create three server objects" << std::endl;
    Server* webServer{Server::createServer(Server::ServerTypes::WEB)};
    Server* mailServer{Server::createServer(Server::ServerTypes::MAIL)};
    Server* ftpServer{Server::createServer(Server::ServerTypes::FTP)};
    Server* firewall{Server::createServer(Server::ServerTypes::FIREWALL)};

    std::cout << std::endl << "Now let's see the object descriptions" << std::endl << std::endl;
    std::cout << webServer->getDescription() << std::endl;
    std::cout << mailServer->getDescription() << std::endl;
    std::cout << ftpServer->getDescription() << std::endl;
    std::cout << firewall->getDescription() << std::endl << std::endl;

    // user is responsible for deleting the factory created objects
    delete webServer;
    webServer = nullptr;
    delete mailServer;
    mailServer = nullptr;
    delete ftpServer;
    ftpServer = nullptr;
    delete firewall;
    firewall = nullptr;

    return 0;
}
