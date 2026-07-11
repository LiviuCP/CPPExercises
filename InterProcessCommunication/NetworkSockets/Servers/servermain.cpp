#include <iostream>

#include "server.h"

import utils;

int main()
{
    Utilities::clearScreen();

    Server server{1024, 9010, false, std::vector<int>{2, 5, -2, 6, 7, -1, 9, 10, -4, 3, 2, 14}, "S1"};
    std::cout << "Server " << server.getName() << " setup" << std::endl;
    server.listenForConnections();

    return 0; // function does not return, it's added just for consistency reasons
}
