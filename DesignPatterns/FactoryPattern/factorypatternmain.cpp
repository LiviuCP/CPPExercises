/* This app shows a basic implmentation of the (abstract) factory design pattern*/

#include <iostream>

#include "server.h"
#include "utils.h"

using namespace std;

int main()
{
    Utilities::clearScreen();

    cout << "Let's create three server objects" << endl;
    Server* webServer{Server::createServer(Server::ServerTypes::WEB)};
    Server* mailServer{Server::createServer(Server::ServerTypes::MAIL)};
    Server* ftpServer{Server::createServer(Server::ServerTypes::FTP)};
    Server* firewall{Server::createServer(Server::ServerTypes::FIREWALL)};

    cout << endl << "Now let's see the object descriptions" << endl << endl;
    cout << webServer->getDescription() << endl;
    cout << mailServer->getDescription() << endl;
    cout << ftpServer->getDescription() << endl;
    cout << firewall->getDescription() << endl << endl;

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
