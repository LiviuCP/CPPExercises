#include "datautils.h"

bool Utilities::convertBitStringToDataWord(const std::string& bitString, DataWord& word)
{
    bool success{false};

    DataWord result;

    const size_t c_WordSize{bitString.size()};

    if (c_WordSize > 0u)
    {
        result.reserve(c_WordSize);

        for (std::string::const_iterator it{bitString.cbegin()}; it != bitString.cend(); ++it)
        {
            if ('0' == *it)
            {
                result.push_back(false);
            }
            else if ('1' == *it)
            {
                result.push_back(true);
            }
            else
            {
                break;
            }
        }

        if (c_WordSize == result.size())
        {
            word = move(result);
            success = true;
        }
    }

    return success;
}

DataWord Utilities::invertDataWord(const DataWord& word)
{
    DataWord result;

    result.reserve(word.size());

    for (const auto& currentBit : word)
    {
        result.push_back(!currentBit);
    }

    return result;
}
