#pragma once

#include <map>

enum class DataTypes
{
    INT = 1, // start from 1 as message type 0 causes faulty reading from queue
    DOUBLE,
    METEODATA,
    DataTypesCount
};

const std::map<DataTypes, std::string> c_DataTypesNames{
    {DataTypes::INT, "integer"}, {DataTypes::DOUBLE, "double"}, {DataTypes::METEODATA, "meteorological data"}};
