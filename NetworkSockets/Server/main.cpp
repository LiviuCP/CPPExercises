#include <iostream>

#include "server.h"

using namespace std;

int main()
{
    Server server{1024, 9010, vector<int>{2, 5, -2, 6, 7, -1, 9, 10, -4, 3, 2, 14}, "S1"};
    server.listenForConnections();

    return 0; // function does not return, it's added just for consistency reasons
}
