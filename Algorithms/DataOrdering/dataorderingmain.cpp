/* This is a small application that illustrates the data ordering theory

   Problem:
   - multiple words of a fixed bit size (e.g. 15 bits) need to be transmitted over the network
   - the words can be transmitted in any order
   - it is possible to invert any of the words (e.g. 01101010 -> 10010101)

   Challenge:
   - find a way to transmit the words that minimizes the total number of bit transitions (Hamming distances kept to a
   minimum)

   Two algorithms will be used:
   - Greedy Min Simplified (GMS): ordering only, no inversion
   - Greedy Min Simplified with inversion: words are ordered and possibly inverted
*/

#include <cassert>
#include <iostream>

#include "dataordering_io.h"
#include "dataorderingengine.h"
#include "utils.h"

using namespace std;

/* Input file contains one or more data sections (separated by an empty row). Each section consists of:
   - header row: number of words and word length (in bits)
   - payload rows: the actual words expressed in binary (one word per row, e.g. 10010100)
*/

static const string c_InFile{Utilities::c_InputOutputDir + "dataorderinginput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "dataorderingoutput.txt"};

void displayResult(ResultType resultType, size_t orderedDataSetsCount, const string& inputFilePath,
                   const string& outputFilePath);

int main()
{
    DataOrderingFileReader fileReader{c_InFile};
    DataOrderingFileWriter fileWriter{c_OutFile};

    Utilities::clearScreen();
    cout << "Reading dataset from input file:\n" << fileReader.getInputFilePath() << "\n\n";

    Result result{ResultType::SUCCESS, std::nullopt};
    std::vector<DataSet> dataSetsToOrder;

    while (result.first == ResultType::SUCCESS)
    {
        result = fileReader.readDataSetFromFile();

        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        if (!result.second.has_value())
        {
            assert(false);
            break;
        }

        cout << "Input is valid: " << result.second->size() << " " << result.second->at(0).size() << "-bit words\n";
        dataSetsToOrder.push_back(*result.second);
    }

    DataOrderingEngine engine;

    for (const auto& dataSet : dataSetsToOrder)
    {
        fileWriter.beginSection();

        engine.setDataSet(dataSet);
        result = fileWriter.writeScenarioOutputToFile("A. Initial order", engine);

        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        engine.performGreedyMinSimplified();
        result = fileWriter.writeScenarioOutputToFile(
            "\n\nB. Scenario 1: Greedy min simplified (GMS) without inversion", engine);

        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        engine.performGreedyMinSimplifiedUsingInversion();
        result = fileWriter.writeScenarioOutputToFile("\n\nC. Scenario 2: Greedy min simplified (GMS) with inversion",
                                                      engine);

        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        fileWriter.endSection();
    }

    displayResult(result.first, dataSetsToOrder.size(), fileReader.getInputFilePath(), fileWriter.getOutputFilePath());

    return 0;
}

void displayResult(ResultType resultType, size_t orderedDataSetsCount, const std::string& inputFilePath,
                   const std::string& outputFilePath)
{
    if (orderedDataSetsCount > 0)
    {
        std::cout << "\nFinished ordering " << orderedDataSetsCount << " dataset(s)!\n\n";
        resultType = ResultType::SUCCESS;
    }

    switch (resultType)
    {
    case ResultType::SUCCESS:
        cout << "Resulting ordering written to file:\n" << outputFilePath << "\n\n";
        break;
    case ResultType::INPUT_FILE_OPENING_ERROR:
        cout << "Error! Cannot open input file:\n" << inputFilePath << "\n\n";
        break;
    case ResultType::OUTPUT_FILE_OPENING_ERROR:
        cout << "Error! Cannot open output file:\n" << outputFilePath << "\n\n";
        break;
    case ResultType::INVALID_INPUT:
        cout << "Invalid or no input in file:\n" << inputFilePath << "\n\n";
        break;
    default:
        assert(false);
        break;
    }
}
