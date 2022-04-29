/* This is a small application that illustrates the data ordering theory

   Problem:
   - multiple words of a fixed bit size (e.g. 15 bits) need to be transmitted over the network
   - the words can be transmitted in any order
   - it is possible to invert any of the words (e.g. 01101010 -> 10010101)

   Challenge:
   - find a way to transmit the words that minimizes the total number of bit transitions (Hamming distances kept to a minimum)

   Two algorithms will be used:
   - Greedy Min Simplified (GMS): ordering only, no inversion
   - Greedy Min Simplified with inversion: words are ordered and possibly inverted
*/

#include <iostream>
#include <fstream>

#include "dataorderingengine.h"
#include "utils.h"

//#include "matrixutils.h" // uncomment if the adjustment matrix needs to be displayed for debugging purposes (otherwise the default ostream operator handles it as boolean)

using namespace std;

/* Input file contains:
   - on the first row: number of words and word length (in bits)
   - on the next rows: the actual words expressed in binary (one word per row, e.g. 10010100)
*/

static const string c_InFile{Utilities::c_InputOutputDir + "dataorderinginput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "dataorderingoutput.txt"};

bool readDataSetFromFile(ifstream& in, DataSet& dataSet);
void writeScenarioOutputToFile(ofstream& out, const DataOrderingEngine& engine);

// writes resulting dataset (ordered and if required inverted) to file
void writeConvertedDataSetToFile(ofstream& out, const DataOrderingEngine& engine);

// original word indexes in dataset reordered
void writeOrderingIndexesToFile(ofstream& out, const OrderingIndexes& indexes);

// used for writing both each data word and the inversion flags to file
void writeBitSetToFile(ofstream& out, const DataWord& word);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    DataSet dataSet;

    const bool c_IsValidInput{readDataSetFromFile(in, dataSet)};

    if (c_IsValidInput)
    {
        cout << "Reading dataset from input file: " << endl << c_InFile << endl << endl;
        cout << "Input is valid: " << dataSet.size() << " " << dataSet.at(0).size() << "-bit words" << endl << endl;

        DataOrderingEngine engine{dataSet};

        out << "Scenario 1: initial order" << endl << endl;
        writeScenarioOutputToFile(out, engine);

        engine.performGreedyMinSimplified();

        out << endl << endl << "Scenario 2: Greedy min simplified (GMS) without inversion" << endl << endl;
        writeScenarioOutputToFile(out, engine);

        engine.performGreedyMinSimplifiedWithInversion();

        out << endl << endl << "Scenario 3: Greedy min simplified (GMS) with inversion" << endl << endl;
        writeScenarioOutputToFile(out, engine);

        cout << "Resulting ordering written to file: " << endl << c_OutFile << endl << endl;
    }
    else
    {
        cout << "Invalid input! Please check file:" << endl << c_InFile << endl << endl;
    }

    return static_cast<int>(!c_IsValidInput);
}

bool readDataSetFromFile(ifstream& in, DataSet& dataSet)
{
    bool isValidSet{false};

    size_t wordsCount{0};
    size_t wordSize{0};

    in >> wordsCount;

    if (!in.fail() && wordsCount > 0)
    {
        in >> wordSize;

        if (!in.fail() && wordSize > 0)
        {
            DataSet tempDataSet;
            tempDataSet.reserve(wordsCount);

            for (size_t currentWordNr{0}; currentWordNr < wordsCount; ++currentWordNr)
            {
                bool isValidWord{false};
                string currentBitString;

                in >> currentBitString;

                if (!in.fail())
                {
                    DataWord currentWord;

                    const bool c_IsValidBinary{Utilities::convertBitStringToDataWord(currentBitString, currentWord)};

                    if (c_IsValidBinary && wordSize == currentWord.size())
                    {
                        tempDataSet.push_back(currentWord);
                        isValidWord = true;
                    }
                }

                if (!isValidWord)
                {
                    break;
                }
            }

            if (wordsCount == tempDataSet.size())
            {
                dataSet = std::move(tempDataSet);
                isValidSet = true;
            }
        }
    }

    return isValidSet;
}

void writeScenarioOutputToFile(ofstream& out, const DataOrderingEngine& engine)
{
    out << "The transmitted data words are: " << endl << endl;

    writeConvertedDataSetToFile(out, engine);

    out << endl << "Transmission order: ";

    writeOrderingIndexesToFile(out, engine.getOrderingIndexes());

    out << endl << "Inversion status: ";

    writeBitSetToFile(out, engine.getInversionFlags());

    out << endl << "Total number of transitions is: ";
    out << engine.getTotalTransitionsCount() << endl;
}

void writeConvertedDataSetToFile(ofstream& out, const DataOrderingEngine& engine)
{
    const DataSet& dataSet{engine.getDataSet()};
    const OrderingIndexes& orderingIndexes{engine.getOrderingIndexes()};
    const InversionFlags& inversionFlags{engine.getInversionFlags()};

    const size_t dataSetSize{dataSet.size()};

    if (dataSetSize == orderingIndexes.size() && dataSetSize == inversionFlags.size())
    {
        DataSet convertedDataSet;
        convertedDataSet.reserve(dataSetSize);

        for (size_t currentWordIndex{0}; currentWordIndex < dataSet.size(); ++currentWordIndex)
        {
            if (inversionFlags.at(currentWordIndex))
            {
                convertedDataSet.push_back(Utilities::invertDataWord(dataSet.at(orderingIndexes.at(currentWordIndex))));
            }
            else
            {
                convertedDataSet.push_back(dataSet.at(orderingIndexes.at(currentWordIndex)));
            }
        }

        for (const auto& currentConvertedWord : convertedDataSet)
        {
            writeBitSetToFile(out, currentConvertedWord);
            out << endl;
        }
    }
    else
    {
        assert(false);
    }
}

void writeOrderingIndexesToFile(ofstream& out, const OrderingIndexes& indexes)
{
    const size_t indexesSize{indexes.size()};

    if (indexesSize > 0)
    {
        for (size_t currentIndex{0}; currentIndex < indexesSize - 1; ++currentIndex)
        {
            out << indexes.at(currentIndex);
            out << ", ";
        }

        out << indexes.at(indexesSize - 1);
    }
}

void writeBitSetToFile(ofstream& out, const DataWord& word)
{
    for (const auto& currentBit : word)
    {
        out << currentBit;
    }
}
