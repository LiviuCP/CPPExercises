#include <iostream>
#include <string>

#include "queuesender.h"
#include "../QueueUtils/queuedataobjects.h"

using namespace std;

const string c_QueueFilename{"/tmp/messagequeue"};

int main()
{
    int firstInt{5}, secondInt{4};
    double firstDouble{5.24}, secondDouble{-2.45};
    MeteoData firstMeteoData{-5, 22.4, 56.2f}, secondMeteoData{23, 2.2, 0.0f};

    QueueSender queueSender{c_QueueFilename};

    cout << "Let's write to data queue" << endl << endl;

    queueSender.writeToQueue(&secondInt, DataTypes::INT);
    cout << "Sent data type: " << c_DataTypesNames.at(DataTypes::INT) << " Value: " << secondInt << endl;

    queueSender.writeToQueue(&firstDouble, DataTypes::DOUBLE);
    cout << "Sent data type: " << c_DataTypesNames.at(DataTypes::DOUBLE) << " Value: " << firstDouble << endl;

    queueSender.writeToQueue(&secondDouble, DataTypes::DOUBLE);
    cout << "Sent data type: " << c_DataTypesNames.at(DataTypes::DOUBLE) << " Value: " << secondDouble << endl;

    queueSender.writeToQueue(&firstMeteoData, DataTypes::METEODATA);
    cout << "Sent data type: " << c_DataTypesNames.at(DataTypes::METEODATA) << " Value: " << firstMeteoData << endl;

    queueSender.writeToQueue(&firstInt, DataTypes::INT);
    cout << "Sent data type: " << c_DataTypesNames.at(DataTypes::INT) << " Value: " << firstInt << endl;

    queueSender.writeToQueue(&secondMeteoData, DataTypes::METEODATA);
    cout << "Sent data type: " << c_DataTypesNames.at(DataTypes::METEODATA) << " Value: " << secondMeteoData << endl;

    cout << endl;

    return 0;
}
