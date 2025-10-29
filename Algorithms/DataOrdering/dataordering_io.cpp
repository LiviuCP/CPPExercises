#include "dataordering_io.h"

DataOrderingFileReader::DataOrderingFileReader(const std::string& inputFilePath)
    : m_InputFilePath{inputFilePath}
{
}

Result DataOrderingFileReader::readDataSetFromFile()
{
    ResultType resultType{ResultType::SUCCESS};
    std::optional<DataSet> resultingDataSet;

    m_In.open(m_InputFilePath);

    if (m_In.is_open())
    {
        DataSet dataSet;
        m_In >> dataSet;

        if (!m_In.fail())
        {
            resultingDataSet = std::move(dataSet);
        }
        else
        {
            resultType = ResultType::INVALID_INPUT;
        }

        m_In.close();
    }
    else
    {
        resultType = ResultType::INPUT_FILE_OPENING_ERROR;
    }

    return {resultType, resultingDataSet};
}

const std::string& DataOrderingFileReader::getInputFilePath() const
{
    return m_InputFilePath;
}

DataOrderingFileWriter::DataOrderingFileWriter(const std::string& outputFilePath, bool fileCleanupRequired)
    : m_OutputFilePath{outputFilePath}
{
    if (fileCleanupRequired)
    {
        m_Out.open(m_OutputFilePath);
        m_Out.close();
    }
}

Result DataOrderingFileWriter::writeScenarioOutputToFile(const std::string& header, const DataOrderingEngine& engine)
{
    ResultType resultType{ResultType::SUCCESS};
    std::optional<DataSet> resultingDataSet;

    m_Out.open(m_OutputFilePath, std::ios::app);

    if (m_Out.is_open())
    {
        resultingDataSet = engine.getOrderedDataSet();

        m_Out << header << "\n\n";
        m_Out << "The transmitted data words are: \n\n";
        m_Out << *resultingDataSet << "\n";
        m_Out << "Transmission order: ";
        m_Out << engine.getOrderingIndexes() << "\n";
        m_Out << "Inversion status: ";
        m_Out << engine.getInversionFlags() << "\n";
        m_Out << "Total number of transitions is: ";
        m_Out << engine.getTotalTransitionsCount() << "\n";

        m_Out.close();
    }
    else
    {
        resultType = ResultType::OUTPUT_FILE_OPENING_ERROR;
    }

    return {resultType, resultingDataSet};
}

const std::string& DataOrderingFileWriter::getOutputFilePath() const
{
    return m_OutputFilePath;
}
