#include <algorithm>
#include <cctype>

#include "datautils.h"

bool Utilities::convertBitStringToDataWord(const std::string& bitString, DataWord& word)
{
    const bool c_IsInputValid{!bitString.empty() && std::all_of(bitString.cbegin(), bitString.cend(), [](auto ch){return ch == '0' || ch == '1';})};

    if (c_IsInputValid)
    {
        word.clear();
        word.resize(bitString.size(), false);
        std::transform(bitString.cbegin(), bitString.cend(), word.begin(), [](char ch){return ch == '1';});
    }

    return c_IsInputValid;
}

DataWord Utilities::invertDataWord(const DataWord& word)
{
    DataWord result(word.size(), false);
    std::transform(word.cbegin(), word.cend(), result.begin(), [](const bool& value){return !value;});

    return result;
}

void Utilities::leftTrimWhiteSpace(std::string& str)
{
    const auto it{std::find_if(str.cbegin(), str.cend(), [](char ch){return !std::isspace(ch);})};
    str.erase(str.cbegin(), it);
}

void Utilities::rightTrimWhiteSpace(std::string& str)
{
    const auto reverseIt{std::find_if(str.crbegin(), str.crend(), [](char ch){return !std::isspace(ch);})};
    const auto it{str.cbegin() + str.size() - std::distance(str.crbegin(), reverseIt)};

    str.erase(it, str.cend());
}

void Utilities::trimWhiteSpace(std::string& str)
{
    leftTrimWhiteSpace(str);
    rightTrimWhiteSpace(str);
}
