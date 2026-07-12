#include <chrono>
#include <iostream>
#include <thread>

#include "client.h"
#include "utils.h"

static const std::string c_InternalLoopbackAddress{"127.0.0.1"};

void displayRetrievedData(const Client& client);

int main(int argc, char* argv[])
{
    Utilities::clearScreen();

    const std::string c_IpAddress{argc == 1 ? c_InternalLoopbackAddress : argv[1]};

    Client client{1024, 9010, c_IpAddress, "C1"};
    std::cout << "Client will connect to server with IP " << client.getIpAddress()
              << " and attempt to retrieve some data" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{1});
    client.retrieveDataFromServer(4);

    displayRetrievedData(client);

    std::this_thread::sleep_for(std::chrono::seconds{1});
    client.setIpAddress("127.0.0.1");
    std::cout << "Now the client will (re)connect to the internal loopback address (" << client.getIpAddress() << ")"
              << " and attempt to retrieve more data" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{1});
    client.retrieveDataFromServer(10);

    displayRetrievedData(client);

    return 0;
}

void displayRetrievedData(const Client& client)
{
    std::this_thread::sleep_for(std::chrono::seconds{1});

    const std::vector<int> clientData{client.getData()};

    std::cout << "Client " << client.getName();

    if (clientData.size() == 0)
    {
        std::cout << " has no data!" << std::endl;
    }
    else
    {
        std::cout << " data is: ";

        for (auto element : client.getData())
        {
            std::cout << element << " ";
        }
    }

    std::cout << std::endl;
}
