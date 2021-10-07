#include <iostream>
#include <thread>
#include <chrono>

#include "../Client/client.h"
#include "utils.h"

using namespace std;

static const string c_InternalLoopbackAddress{"127.0.0.1"};

void executeOperation(Client& client, size_t valuesCount);

int main(int argc, char* argv[])
{
    Utilities::clearScreen();

    const string c_IpAddress{argc == 1 ? c_InternalLoopbackAddress : argv[1]};

    cout << "*** Creating clients and launching them into execution ***" << endl;

    Client firstClient{1024, 9010, c_IpAddress, "C1"};
    Client secondClient{1024, 9010, c_IpAddress, "C2"};

    thread firstClientThread{&executeOperation, ref(firstClient), 4};
    thread secondClientThread{&executeOperation, ref(secondClient), 10};

    firstClientThread.join();
    secondClientThread.join();

    cout << "*** All clients finished executing ***" << endl;

    return 0;
}

void executeOperation(Client& client, size_t requestedValuesCount)
{
    cout << "Client " << client.getName() << " will connect to server with IP " << client.getIpAddress() << "and attempt to retrieve some data" << endl;

    this_thread::sleep_for(chrono::seconds{1});
    client.retrieveDataFromServer(requestedValuesCount);

    this_thread::sleep_for(chrono::seconds{1});
    cout << "The data retrieved by client " << client.getName() << " from server is: ";

    for (auto element : client.getData())
    {
        cout << element << " ";
    }

    cout << endl;
}
