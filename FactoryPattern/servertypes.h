#ifndef SERVERTYPES_H
#define SERVERTYPES_H

#include <string>

#include "server.h"

class WebServer : public Server
{
public:
    std::string getDescription() const;
};

class MailServer : public Server
{
public:
    std::string getDescription() const;
};

class FTPServer : public Server
{
public:
    std::string getDescription() const;
};

#endif // SERVERTYPES_H
