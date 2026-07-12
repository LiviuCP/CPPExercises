#include <chrono>
#include <iostream>
#include <thread>

#include "client.h"
#include "utils.h"

static const std::string c_InternalLoopbackAddress{"127.0.0.1"};

void executeOperation(Client& client, size_t valuesCount);

int main(int argc, char* argv[])
{
    Utilities::clearScreen();

    const std::string c_IpAddress{argc == 1 ? c_InternalLoopbackAddress : argv[1]};

    std::cout << "*** Creating clients and launching them into execution ***" << std::endl;

    Client firstClient{1024, 9010, c_IpAddress, "C1"};
    Client secondClient{1024, 9010, c_IpAddress, "C2"};

    std::thread firstClientThread{&executeOperation, std::ref(firstClient), 4};
    std::thread secondClientThread{&executeOperation, std::ref(secondClient), 10};

    firstClientThread.join();
    secondClientThread.join();

    std::cout << "*** All clients finished executing ***" << std::endl;

    return 0;
}

void executeOperation(Client& client, size_t requestedValuesCount)
{
    std::cout << "Client " << client.getName() << " will connect to server with IP " << client.getIpAddress()
              << "and attempt to retrieve some data" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds{1});
    client.retrieveDataFromServer(requestedValuesCount);

    std::this_thread::sleep_for(std::chrono::seconds{1});
    std::cout << "The data retrieved by client " << client.getName() << " from server is: ";

    for (auto element : client.getData())
    {
        std::cout << element << " ";
    }

    std::cout << std::endl;
}
