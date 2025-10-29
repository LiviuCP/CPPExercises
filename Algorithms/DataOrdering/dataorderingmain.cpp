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
#include <cassert>

#include "dataorderingengine.h"
#include "dataordering_io.h"
#include "utils.h"

//#include "matrixutils.h" // uncomment if the adjustment matrix needs to be displayed for debugging purposes (otherwise the default ostream operator handles it as boolean)

using namespace std;

/* Input file contains:
   - on the first row: number of words and word length (in bits)
   - on the next rows: the actual words expressed in binary (one word per row, e.g. 10010100)
*/

static const string c_InFile{Utilities::c_InputOutputDir + "dataorderinginput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "dataorderingoutput.txt"};

void handleResult(ResultType resultType, const string& inputFilePath, const string& outputFilePath);

int main()
{
    DataOrderingFileReader fileReader{c_InFile};
    DataOrderingFileWriter fileWriter{c_OutFile};

    Utilities::clearScreen();
    cout << "Reading dataset from input file:\n" << fileReader.getInputFilePath() << "\n\n";

    Result result{fileReader.readDataSetFromFile()};

    do
    {
        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        if (!result.second.has_value())
        {
            assert(false);
            break;
        }

        const DataSet& dataSet{*result.second};
        cout << "Input is valid: " << dataSet.size() << " " << dataSet.at(0).size() << "-bit words\n\n";

        DataOrderingEngine engine{dataSet};
        result = fileWriter.writeScenarioOutputToFile("Scenario 1: initial order", engine);

        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        engine.performGreedyMinSimplified();
        result = fileWriter.writeScenarioOutputToFile("\n\nScenario 2: Greedy min simplified (GMS) without inversion", engine);

        if (result.first != ResultType::SUCCESS)
        {
            break;
        }

        engine.performGreedyMinSimplifiedWithInversion();
        result = fileWriter.writeScenarioOutputToFile("\n\nScenario 3: Greedy min simplified (GMS) with inversion", engine);
    }
    while(false);

    handleResult(result.first, fileReader.getInputFilePath(), fileWriter.getOutputFilePath());

    return static_cast<int>(result.first);
}

void handleResult(ResultType resultType, const std::string& inputFilePath, const std::string& outputFilePath)
{
    switch (resultType)
    {
    case ResultType::SUCCESS:
        cout << "Resulting ordering written to file:\n" << outputFilePath << "\n\n";
        break;
    case ResultType::INPUT_FILE_OPENING_ERROR:
        cout << "Invalid input! Please check file:\n" << inputFilePath << "\n\n";
        break;
    case ResultType::OUTPUT_FILE_OPENING_ERROR:
        cout << "Error! Cannot open output file:\n" << outputFilePath << "\n\n";
        break;
    case ResultType::INVALID_INPUT:
        cout << "Invalid input in file:\n" << inputFilePath << "\n\n";
        break;
    default:
        assert(false);
        break;
    }
}
