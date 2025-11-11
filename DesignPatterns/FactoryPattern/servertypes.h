#pragma once

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

class Firewall : public Server
{
public:
    std::string getDescription() const;
};
