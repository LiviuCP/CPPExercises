#pragma once

#include <condition_variable>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ICSVAggregator;

class CSVParsingQueue
{
public:
    CSVParsingQueue(ICSVAggregator& csvAggregator, size_t threadsCount = std::thread::hardware_concurrency());
    ~CSVParsingQueue();

    bool parseCSVFiles(const std::vector<std::filesystem::path>& csvFilePaths);

    // all files from the queue get parsed, then the threads are stopped
    void finishParsingAndStop();

    // stops threads once all currently running jobs are done, doesn't wait for the queue to get empty
    void stop() noexcept;

private:
    void _createParsingThreads();
    void _enqueueParsingTask(const std::filesystem::path& csvFilePath);
    void _parseCSVFile(const std::filesystem::path& inputCSVPath);

    ICSVAggregator& m_CsvAggregator;
    std::vector<std::thread> m_ThreadPool;
    std::queue<std::filesystem::path> m_CsvFilePathsQueue;
    std::mutex m_QueueMutex;
    std::condition_variable m_QueueConditionVariable;
    size_t m_ThreadsCount;
    bool m_ShouldStopParsing;
};
