/* General purpose data type(def)s and conversion functions*/
#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <vector>
#include <string>

using DataWord = std::vector<bool>;
using DataSet = std::vector<DataWord>;

namespace Utilities
{
    bool convertBitStringToDataWord(const std::string& bitString, DataWord& word);
    DataWord invertDataWord(const DataWord& word);
}

#endif // DATAUTILS_H
