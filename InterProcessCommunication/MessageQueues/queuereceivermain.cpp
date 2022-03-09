#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/wait.h>

#include "queuereceiver.h"
#include "queuedataobjects.h"
#include "utils.h"

using namespace std;

const string c_QueueFilename{"/tmp/messagequeue"};

/* run the sender app first and then the receiver (separate terminals) */

int main()
{
    Utilities::clearScreen();

    int intBuffer;
    double doubleBuffer;
    MeteoData meteoDataBuffer;

    cout << "Let's read from data queue (receivers wait and listen to queue if data is not available)" << endl << endl;

    const int processID{fork()};

    if (processID == 0)
    {
        QueueReceiver childQueueReceiver{c_QueueFilename, true};

        childQueueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
        cout << "Child process " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << endl;

        childQueueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
        cout << "Child process " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer << endl;

        childQueueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
        cout << "Child process " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer << endl;

        sleep(2); // simulate waiting for the child to exit so parent can remove the queue

        cout << "Child process is exiting" << endl;

        _exit(0); // inform parent process of exit so the message queue can be removed by parent
    }
    else
    {
        QueueReceiver parentQueueReceiver{c_QueueFilename, true};

        parentQueueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
        cout << "Parent process " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer << endl;

        parentQueueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
        cout << "Parent process " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << endl;

        parentQueueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
        cout << "Parent process " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer << endl;

        cout << "Parent process is waiting for the child to exit" << endl;
        wait(nullptr);

        cout << "Child exited. Parent process is now removing the queue and exiting." << endl << endl;
        parentQueueReceiver.removeQueue();

        exit(0);
    }

    return 0; // just for following the best practices (main() function should return something) - actually not executed
}
