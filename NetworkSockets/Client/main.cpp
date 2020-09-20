#include <iostream>

#include "client.h"

using namespace std;

static const string c_InternalLoopbackAddress{"127.0.0.1"};

int main(int argc, char* argv[])
{
    const string c_IpAddress{argc == 1 ? c_InternalLoopbackAddress : argv[1]};

    Client client{1024, 9010, c_IpAddress, "C1"};
    client.retrieveDataFromServer(4);

    cout << endl << "The data retrieved by client " << client.getName() << " from server is: ";
    for (auto element : client.getData())
    {
        cout << element << " ";
    }
    cout << endl << endl;

    return 0;
}
