#include <algorithm>
#include <cctype>
#include <numeric>

#include "datautils.h"

bool Utilities::convertBitStringToDataWord(const std::string& bitString, DataWord& word)
{
    const bool c_IsInputValid{!bitString.empty() && std::all_of(bitString.cbegin(), bitString.cend(),
                                                                [](auto ch) { return ch == '0' || ch == '1'; })};

    if (c_IsInputValid)
    {
        word.clear();
        word.resize(bitString.size(), false);
        std::transform(bitString.cbegin(), bitString.cend(), word.begin(), [](char ch) { return ch == '1'; });
    }

    return c_IsInputValid;
}

DataWord Utilities::invertDataWord(const DataWord& word)
{
    DataWord result(word.size(), false);
    std::transform(word.cbegin(), word.cend(), result.begin(), [](const bool& value) { return !value; });

    return result;
}

void Utilities::leftTrimWhiteSpace(std::string& str)
{
    const auto it{std::find_if(str.cbegin(), str.cend(), [](char ch) { return !std::isspace(ch); })};
    str.erase(str.cbegin(), it);
}

void Utilities::rightTrimWhiteSpace(std::string& str)
{
    const auto reverseIt{std::find_if(str.crbegin(), str.crend(), [](char ch) { return !std::isspace(ch); })};
    const auto it{str.cbegin() + str.size() - std::distance(str.crbegin(), reverseIt)};

    str.erase(it, str.cend());
}

void Utilities::trimWhiteSpace(std::string& str)
{
    leftTrimWhiteSpace(str);
    rightTrimWhiteSpace(str);
}

std::istream& operator>>(std::istream& in, DataSet& dataSet)
{
    size_t wordsCount{0};
    size_t wordSize{0};
    DataSet tempDataSet;

    do
    {
        in >> wordsCount;

        if (in.fail() || wordsCount == 0)
        {
            break;
        }

        in >> wordSize;

        if (in.fail() || wordSize == 0)
        {
            wordsCount = 0;
            break;
        }

        tempDataSet.reserve(wordsCount);
    } while (false);

    for (size_t currentWordNr{0}; currentWordNr < wordsCount; ++currentWordNr)
    {
        std::string currentBitString;
        in >> currentBitString;

        if (in.fail())
        {
            break;
        }

        DataWord currentWord;
        const bool c_IsValidBinary{Utilities::convertBitStringToDataWord(currentBitString, currentWord)};

        if (c_IsValidBinary && wordSize == currentWord.size())
        {
            tempDataSet.emplace_back(std::move(currentWord));
            continue;
        }

        break;
    }

    if (!tempDataSet.empty() && wordsCount == tempDataSet.size())
    {
        dataSet = std::move(tempDataSet);
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataSet& dataSet)
{
    const size_t c_CharsCount = dataSet.size() + std::accumulate(dataSet.cbegin(), dataSet.cend(), 0,
                                                                 [](const size_t partialSum, const auto& toAdd) {
                                                                     return partialSum + toAdd.size();
                                                                 });
    std::string charsToWrite;

    charsToWrite.reserve(c_CharsCount);

    for (const auto& word : dataSet)
    {
        std::transform(word.cbegin(), word.cend(), std::back_inserter(charsToWrite),
                       [](bool value) { return value ? '1' : '0'; });
        charsToWrite.push_back('\n');
    }

    out << charsToWrite;

    return out;
}

std::ostream& operator<<(std::ostream& out, const DataWord& word)
{
    std::string charsToWrite;
    charsToWrite.reserve(word.size());

    std::transform(word.cbegin(), word.cend(), std::back_inserter(charsToWrite),
                   [](bool value) { return value ? '1' : '0'; });
    out << charsToWrite;

    return out;
}

std::ostream& operator<<(std::ostream& out, const SizeVector& indexes)
{
    const size_t c_IndexesSize{indexes.size()};

    if (c_IndexesSize > 0)
    {
        for (size_t currentIndex{0}; currentIndex < c_IndexesSize - 1; ++currentIndex)
        {
            out << indexes.at(currentIndex) << ", ";
        }

        out << indexes.at(c_IndexesSize - 1);
    }

    return out;
}
