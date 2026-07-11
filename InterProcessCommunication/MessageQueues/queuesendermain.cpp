#include <iostream>
#include <string>

#include <unistd.h>

#include "queuedataobjects.h"
#include "queuesender.h"

import utils;

const std::string c_QueueFilename{"/tmp/messagequeue"};

/* run the sender app first and then the receiver (separate terminals) */

int main()
{
    Utilities::clearScreen();

    int firstInt{5}, secondInt{4};
    double firstDouble{5.24}, secondDouble{-2.45};
    MeteoData firstMeteoData{-5, 22.4, 56.2f}, secondMeteoData{23, 2.2, 0.0f};

    std::cout << "Let's write to data queue" << std::endl << std::endl;
    sleep(5); // for being able to start the receiver program before data is being sent (if sender program had been
              // started first)

    const int processID{fork()};

    if (processID == 0)
    {
        QueueSender childQueueSender{c_QueueFilename};

        sleep(1);
        childQueueSender.writeToQueue(&secondInt, DataTypes::INT);
        std::cout << "Child process sent data type: " << c_DataTypesNames.at(DataTypes::INT) << " Value: " << secondInt
                  << std::endl;

        sleep(2);
        childQueueSender.writeToQueue(&secondDouble, DataTypes::DOUBLE);
        std::cout << "Child process sent data type: " << c_DataTypesNames.at(DataTypes::DOUBLE)
                  << " Value: " << secondDouble << std::endl;

        sleep(3); // simulate a longer operation before sending this object type
        childQueueSender.writeToQueue(&secondMeteoData, DataTypes::METEODATA);
        std::cout << "Child process sent data type: " << c_DataTypesNames.at(DataTypes::METEODATA)
                  << " Value: " << secondMeteoData << std::endl;

        exit(0);
    }
    else
    {
        QueueSender parentQueueSender{c_QueueFilename};

        sleep(2);
        parentQueueSender.writeToQueue(&firstDouble, DataTypes::DOUBLE);
        std::cout << "Parent process sent data type: " << c_DataTypesNames.at(DataTypes::DOUBLE)
                  << " Value: " << firstDouble << std::endl;

        sleep(3); // simulate a longer operation before sending this object type
        parentQueueSender.writeToQueue(&firstMeteoData, DataTypes::METEODATA);
        std::cout << "Parent process sent data type: " << c_DataTypesNames.at(DataTypes::METEODATA)
                  << " Value: " << firstMeteoData << std::endl;

        sleep(1);
        parentQueueSender.writeToQueue(&firstInt, DataTypes::INT);
        std::cout << "Parent process sent data type: " << c_DataTypesNames.at(DataTypes::INT) << " Value: " << firstInt
                  << std::endl;

        sleep(1); // for a nicer output
        std::cout << std::endl;

        exit(0);
    }

    return 0; // just for following the best practices (main() function should return something) - actually not executed
}
