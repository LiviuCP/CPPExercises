#include <iostream>
#include <string>

#include <unistd.h>

#include "queuedataobjects.h"
#include "queuesender.h"
#include "utils.h"

using namespace std;

const string c_QueueFilename{"/tmp/messagequeue"};

/* run the sender app first and then the receiver (separate terminals) */

int main()
{
    Utilities::clearScreen();

    int firstInt{5}, secondInt{4};
    double firstDouble{5.24}, secondDouble{-2.45};
    MeteoData firstMeteoData{-5, 22.4, 56.2f}, secondMeteoData{23, 2.2, 0.0f};

    cout << "Let's write to data queue" << endl << endl;
    sleep(5); // for being able to start the receiver program before data is being sent (if sender program had been
              // started first)

    const int processID{fork()};

    if (processID == 0)
    {
        QueueSender childQueueSender{c_QueueFilename};

        sleep(1);
        childQueueSender.writeToQueue(&secondInt, DataTypes::INT);
        cout << "Child process sent data type: " << c_DataTypesNames.at(DataTypes::INT) << " Value: " << secondInt
             << endl;

        sleep(2);
        childQueueSender.writeToQueue(&secondDouble, DataTypes::DOUBLE);
        cout << "Child process sent data type: " << c_DataTypesNames.at(DataTypes::DOUBLE) << " Value: " << secondDouble
             << endl;

        sleep(3); // simulate a longer operation before sending this object type
        childQueueSender.writeToQueue(&secondMeteoData, DataTypes::METEODATA);
        cout << "Child process sent data type: " << c_DataTypesNames.at(DataTypes::METEODATA)
             << " Value: " << secondMeteoData << endl;

        exit(0);
    }
    else
    {
        QueueSender parentQueueSender{c_QueueFilename};

        sleep(2);
        parentQueueSender.writeToQueue(&firstDouble, DataTypes::DOUBLE);
        cout << "Parent process sent data type: " << c_DataTypesNames.at(DataTypes::DOUBLE) << " Value: " << firstDouble
             << endl;

        sleep(3); // simulate a longer operation before sending this object type
        parentQueueSender.writeToQueue(&firstMeteoData, DataTypes::METEODATA);
        cout << "Parent process sent data type: " << c_DataTypesNames.at(DataTypes::METEODATA)
             << " Value: " << firstMeteoData << endl;

        sleep(1);
        parentQueueSender.writeToQueue(&firstInt, DataTypes::INT);
        cout << "Parent process sent data type: " << c_DataTypesNames.at(DataTypes::INT) << " Value: " << firstInt
             << endl;

        sleep(1); // for a nicer output
        cout << endl;

        exit(0);
    }

    return 0; // just for following the best practices (main() function should return something) - actually not executed
}
