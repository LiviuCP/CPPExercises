#pragma once

#include <string>
#include <fstream>

#include "dataorderingengine.h"

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

    void setInputFilePath(const std::string& inputFilePath);
    const std::string& getInputFilePath() const;

private:
    std::ifstream m_In;
    std::string m_InputFilePath;
};

class DataOrderingFileWriter
{
public:
    DataOrderingFileWriter(const std::string& outputFilePath);

    Result writeScenarioOutputToFile(const std::string& header, const DataOrderingEngine& engine);
    void beginSection();
    void endSection();

    void setOutputFilePath(const std::string& outputFilePath);
    const std::string& getOutputFilePath() const;

private:
    std::ofstream m_Out;
    std::string m_OutputFilePath;
    size_t m_SectionNumber;
};
