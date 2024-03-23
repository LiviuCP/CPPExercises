#pragma once

#include <mutex>
#include <map>
#include "icsvaggregator.h"

class CSVAggregator : public ICSVAggregator
{
public:
    CSVAggregator() = delete;
    explicit CSVAggregator(const std::string& outputFilePath);
    void storeHostData(const std::vector<Data::HostNameAndInfo>& info) override;
    bool writeDataToOutputCSV();

private:
    std::mutex m_DataAggregationMutex;
    std::map<std::string, Data::HostInfo> m_Info;
    std::string m_OutputFilePath;
};
