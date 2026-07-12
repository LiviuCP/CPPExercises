#include <iostream>
#include <string>

#include <sys/wait.h>
#include <unistd.h>

#include "queuedataobjects.h"
#include "queuereceiver.h"
#include "utils.h"

const std::string c_QueueFilename{"/tmp/messagequeue"};

/* run the sender app first and then the receiver (separate terminals) */

int main()
{
    Utilities::clearScreen();

    int intBuffer;
    double doubleBuffer;
    MeteoData meteoDataBuffer;

    std::cout << "Let's read from data queue (receivers wait and listen to queue if data is not available)" << std::endl
              << std::endl;

    const int processID{fork()};

    if (processID == 0)
    {
        QueueReceiver childQueueReceiver{c_QueueFilename, true};

        childQueueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
        std::cout << "Child process " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << std::endl;

        childQueueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
        std::cout << "Child process " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer
                  << std::endl;

        childQueueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
        std::cout << "Child process " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer
                  << std::endl;

        sleep(2); // simulate waiting for the child to exit so parent can remove the queue

        std::cout << "Child process is exiting" << std::endl;

        _exit(0); // inform parent process of exit so the message queue can be removed by parent
    }
    else
    {
        QueueReceiver parentQueueReceiver{c_QueueFilename, true};

        parentQueueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
        std::cout << "Parent process " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer
                  << std::endl;

        parentQueueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
        std::cout << "Parent process " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << std::endl;

        parentQueueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
        std::cout << "Parent process " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer
                  << std::endl;

        std::cout << "Parent process is waiting for the child to exit" << std::endl;
        wait(nullptr);

        std::cout << "Child exited. Parent process is now removing the queue and exiting." << std::endl << std::endl;
        parentQueueReceiver.removeQueue();

        exit(0);
    }

    return 0; // just for following the best practices (main() function should return something) - actually not executed
}
