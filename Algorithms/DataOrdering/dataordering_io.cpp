#include "dataordering_io.h"

DataOrderingFileReader::DataOrderingFileReader(const std::string& inputFilePath)
{
    setInputFilePath(inputFilePath);
}

Result DataOrderingFileReader::readDataSetFromFile()
{
    ResultType resultType{ResultType::SUCCESS};
    std::optional<DataSet> resultingDataSet;

    if (!m_In.is_open())
    {
        m_In.open(m_InputFilePath);
    }

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
    }
    else
    {
        resultType = ResultType::INPUT_FILE_OPENING_ERROR;
    }

    return {resultType, resultingDataSet};
}

void DataOrderingFileReader::setInputFilePath(const std::string& inputFilePath)
{
    if (m_InputFilePath != inputFilePath)
    {
        if (m_In.is_open())
        {
            m_In.close();
        }

        m_InputFilePath = inputFilePath;
    }
}

const std::string& DataOrderingFileReader::getInputFilePath() const
{
    return m_InputFilePath;
}

DataOrderingFileWriter::DataOrderingFileWriter(const std::string& outputFilePath)
{
    setOutputFilePath(outputFilePath);
}

Result DataOrderingFileWriter::writeScenarioOutputToFile(const std::string& header, const DataOrderingEngine& engine)
{
    ResultType resultType{ResultType::SUCCESS};
    std::optional<DataSet> resultingDataSet;

    if (!m_Out.is_open())
    {
        m_Out.open(m_OutputFilePath);
    }

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
    }
    else
    {
        resultType = ResultType::OUTPUT_FILE_OPENING_ERROR;
    }

    return {resultType, resultingDataSet};
}

void DataOrderingFileWriter::beginSection()
{
    if (!m_Out.is_open())
    {
        m_Out.open(m_OutputFilePath);
    }

    if (m_Out.is_open())
    {
        m_Out << "******************************** Section " << m_SectionNumber << " ********************************\n\n";
    }
}

void DataOrderingFileWriter::endSection()
{
    if (m_Out.is_open())
    {
        m_Out << "\n\n";
        ++m_SectionNumber;
    }
}

void DataOrderingFileWriter::setOutputFilePath(const std::string& outputFilePath)
{
    if (m_OutputFilePath != outputFilePath)
    {
        if (m_Out.is_open())
        {
            m_Out.close();
        }

        m_SectionNumber = 1;
        m_OutputFilePath = outputFilePath;
    }
}

const std::string& DataOrderingFileWriter::getOutputFilePath() const
{
    return m_OutputFilePath;
}
