#include <fstream>

#include "csvaggregator.h"

CSVAggregator::CSVAggregator(const std::filesystem::path& outputFilePath)
    : m_OutputFilePath{outputFilePath}
{
}

void CSVAggregator::storeHostData(const std::vector<Data::HostNameAndInfo>& info)
{
    std::lock_guard<std::mutex> lock{m_DataAggregationMutex};

    for (const auto& hostNameAndInfo : info)
    {
        m_Info[hostNameAndInfo.first] = hostNameAndInfo.second;
    }
}

bool CSVAggregator::writeDataToOutputCSV()
{
    bool success{false};

    try
    {
        std::ofstream out{m_OutputFilePath};
        success = out.is_open();

        if (success)
        {
            for (const auto& info : m_Info)
            {
                out << info.first << "," << info.second.m_MacAddress << "," << info.second.m_IpAddress << ","
                    << info.second.m_IpClass << "\n";
            }
        }
    }
    catch (std::ios_base::failure&)
    {
        success = false;
    }

    return success;
}
