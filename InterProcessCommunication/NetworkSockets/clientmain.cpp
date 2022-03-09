#include <iostream>
#include <thread>
#include <chrono>

#include "client.h"
#include "utils.h"

using namespace std;

static const string c_InternalLoopbackAddress{"127.0.0.1"};

void displayRetrievedData(const Client& client);

int main(int argc, char* argv[])
{
    Utilities::clearScreen();

    const string c_IpAddress{argc == 1 ? c_InternalLoopbackAddress : argv[1]};

    Client client{1024, 9010, c_IpAddress, "C1"};
    cout << "Client will connect to server with IP " << client.getIpAddress() << " and attempt to retrieve some data" << endl;
    this_thread::sleep_for(chrono::seconds{1});
    client.retrieveDataFromServer(4);

    displayRetrievedData(client);

    this_thread::sleep_for(chrono::seconds{1});
    client.setIpAddress("127.0.0.1");
    cout << "Now the client will (re)connect to the internal loopback address (" << client.getIpAddress() << ")" << " and attempt to retrieve more data" << endl;
    this_thread::sleep_for(chrono::seconds{1});
    client.retrieveDataFromServer(10);

    displayRetrievedData(client);

    return 0;
}

void displayRetrievedData(const Client& client)
{
    this_thread::sleep_for(chrono::seconds{1});

    const std::vector<int> clientData{client.getData()};

    cout << "Client " << client.getName();

    if (clientData.size() == 0)
    {
        cout << " has no data!" << endl;
    }
    else
    {
        cout << " data is: ";

        for (auto element : client.getData())
        {
            cout << element << " ";
        }
    }

    cout << endl;
}
