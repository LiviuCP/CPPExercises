#pragma once

#include <filesystem>
#include <map>
#include <mutex>

#include "icsvaggregator.h"

class CSVAggregator : public ICSVAggregator
{
public:
    CSVAggregator() = delete;
    explicit CSVAggregator(const std::filesystem::path& outputFilePath);
    void storeHostData(const std::vector<Data::HostNameAndInfo>& info) override;
    bool writeDataToOutputCSV();

private:
    std::mutex m_DataAggregationMutex;
    std::map<std::string, Data::HostInfo> m_Info;
    std::filesystem::path m_OutputFilePath;
};
