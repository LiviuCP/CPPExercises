#include <iostream>
#include <string>

#include "queuereceiver.h"
#include "../QueueUtils/queuedataobjects.h"

using namespace std;

const string c_QueueFilename{"/tmp/messagequeue"};

int main()
{
    int intBuffer;
    double doubleBuffer;
    MeteoData meteoDataBuffer;

    QueueReceiver queueReceiver{c_QueueFilename};
    cout << "Let's read from data queue" << endl << endl;

    queueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
    cout << "First " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer << endl;

    queueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
    cout << "First " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << endl;

    queueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
    cout << "First " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer << endl;

    queueReceiver.readFromQueue(&meteoDataBuffer, DataTypes::METEODATA);
    cout << "Second " << c_DataTypesNames.at(DataTypes::METEODATA) << " read is: " << meteoDataBuffer << endl;

    queueReceiver.readFromQueue(&doubleBuffer, DataTypes::DOUBLE);
    cout << "Second " << c_DataTypesNames.at(DataTypes::DOUBLE) << " read is: " << doubleBuffer << endl;

    queueReceiver.readFromQueue(&intBuffer, DataTypes::INT);
    cout << "Second " << c_DataTypesNames.at(DataTypes::INT) << " read is: " << intBuffer << endl;

    cout << endl;

    return 0;
}
