#include <iostream>
#include <string>

#include <unistd.h>

#include "queuereceiver.h"
#include "../QueueUtils/queuedataobjects.h"

using namespace std;

const string c_QueueFilename{"/tmp/messagequeue"};

/* run the sender app first and then the receiver (separate terminals) */

int main()
{
    int intBuffer;
    double doubleBuffer;
    MeteoData meteoDataBuffer;

    cout << "Let's read from data queue" << endl << endl;

    const int processID{fork()};

    if (processID == 0)
    {
        QueueReceiver childQueueReceiver{c_QueueFilename};

        sleep(1);
        childQueueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
        cout << "Child process " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer << endl;

        sleep(1);
        childQueueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
        cout << "Child process " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << endl;

        sleep(1);
        childQueueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
        cout << "Child process " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer << endl;
    }
    else
    {
        QueueReceiver parentQueueReceiver{c_QueueFilename};

        sleep(1);
        parentQueueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
        cout << "Parent process " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer << endl;

        sleep(1);
        parentQueueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
        cout << "Parent process " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer << endl;

        sleep(1);
        parentQueueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
        cout << "Parent process " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << endl;

        sleep(1);

        parentQueueReceiver.removeQueue();
        cout << endl << "Parent process removed queue" << endl << endl;
    }

    return 0;
}
