#include "servertypes.h"

std::string WebServer::getDescription() const
{
    return "The object is a web server and is used for handling web access!";
}

std::string MailServer::getDescription() const
{
    return "The object is a mail server and is used for handling e-mail traffic!";
}

std::string FTPServer::getDescription() const
{
    return "The object is a FTP server and is used for retrieving data files!";
}

std::string Firewall::getDescription() const
{
    return "The object is a firewall server and is used for protecting the network from unauthorized access!";
}
