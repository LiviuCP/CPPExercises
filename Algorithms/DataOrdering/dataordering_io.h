#ifndef DATAORDERING_IO_H
#define DATAORDERING_IO_H

#include <string>
#include <fstream>

#include "dataorderingengine.h"
#include "datautils.h"

enum class ResultType
{
    SUCCESS = 0,
    INPUT_FILE_OPENING_ERROR,
    OUTPUT_FILE_OPENING_ERROR,
    INVALID_INPUT
};

using Result = std::pair<ResultType, std::optional<DataSet>>;

class DataOrderingFileReader
{
public:
    DataOrderingFileReader(const std::string& inputFilePath);

    Result readDataSetFromFile();
    const std::string& getInputFilePath() const;

private:
    std::ifstream m_In;
    std::string m_InputFilePath;
};

class DataOrderingFileWriter
{
public:
    DataOrderingFileWriter(const std::string& outputFilePath, bool fileCleanupRequired = true);

    Result writeScenarioOutputToFile(const std::string& header, const DataOrderingEngine& engine);
    const std::string& getOutputFilePath() const;

private:
    std::ofstream m_Out;
    std::string m_OutputFilePath;
};

#endif // DATAORDERING_IO_H
