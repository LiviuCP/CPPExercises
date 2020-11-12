#include <iostream>

#include "server.h"

using namespace std;

int main()
{
    Server server{1024, 9010, false, vector<int>{2, 5, -2, 6, 7, -1, 9, 10, -4, 3, 2, 14}, "S1"};
    cout << "Server " << server.getName() << " setup" << endl;
    server.listenForConnections();

    return 0; // function does not return, it's added just for consistency reasons
}
