/* This app shows a basic implmentation of the (abstract) factory design pattern*/

#include <iostream>

#include "server.h"

using namespace std;

int main()
{
    cout << "Let's create three server objects" << endl;
    Server* webServer{Server::createServer(Server::ServerTypes::WEB)};
    Server* mailServer{Server::createServer(Server::ServerTypes::MAIL)};
    Server* ftpServer{Server::createServer(Server::ServerTypes::FTP)};

    cout << endl << "Now let's see the object descriptions" << endl << endl;
    cout << webServer->getDescription() << endl;
    cout << mailServer->getDescription() << endl;
    cout << ftpServer->getDescription() << endl << endl;

    // user is responsible for deleting the factory created objects
    delete webServer;
    webServer = nullptr;
    delete mailServer;
    mailServer = nullptr;
    delete ftpServer;
    ftpServer = nullptr;

    return 0;
}
