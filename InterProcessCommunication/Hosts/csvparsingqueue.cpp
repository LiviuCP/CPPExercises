#include <chrono>
#include <iostream>
#include <cassert>

#include "csvparser.h"
#include "icsvaggregator.h"
#include "apputils.h"
#include "csvparsingqueue.h"

CSVParsingQueue::CSVParsingQueue(ICSVAggregator& csvAggregator, size_t threadsCount)
    : m_CsvAggregator{csvAggregator}
    , m_ThreadsCount{threadsCount}
    , m_ShouldStopParsing{false}
{
    assert(threadsCount > 0);
    bool success{true};

    try
    {
        _createParsingThreads();
    }
    catch(std::system_error&)
    {
        success = false;
        std::cerr << "An error occurred! Unable to start parsing\n";
    }

    if (!success)
    {
        stop();
    }
}

CSVParsingQueue::~CSVParsingQueue()
{
    stop();
}

bool CSVParsingQueue::parseCSVFiles(const std::vector<std::filesystem::path>& csvFilePaths)
{
    if (!m_ShouldStopParsing)
    {
        for (const auto& path : csvFilePaths)
        {
            if (Utils::isCsvFilePath(path))
            {
                _enqueueParsingTask(path);
            }
        }
    }

    return !m_ShouldStopParsing;
}

void CSVParsingQueue::finishParsingAndStop()
{
    if (!m_ShouldStopParsing)
    {
        while (!m_CsvFilePathsQueue.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }

        stop();
    }
}

void CSVParsingQueue::stop() noexcept
{
    {
        std::unique_lock<std::mutex> lock{m_QueueMutex};
        m_ShouldStopParsing = true;
    }

    m_QueueConditionVariable.notify_all();

    for (auto& thread : m_ThreadPool)
    {
        try
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        catch(std::system_error&)
        {
            // no action required, just catch the exception (should not propagate outside this method as it is also called by destructor)
        }
    }
}

void CSVParsingQueue::_createParsingThreads()
{
    for (size_t threadNumber{0}; threadNumber < m_ThreadsCount; ++threadNumber)
    {
        m_ThreadPool.emplace_back([this](){
            for (;;)
            {
                std::filesystem::path csvPath;

                {
                    std::unique_lock<std::mutex> lock{m_QueueMutex};
                    m_QueueConditionVariable.wait(lock, [this](){return !m_CsvFilePathsQueue.empty() || m_ShouldStopParsing;});

                    if (m_CsvFilePathsQueue.empty() || m_ShouldStopParsing)
                    {
                        break;
                    }

                    csvPath = std::move(m_CsvFilePathsQueue.front());
                    m_CsvFilePathsQueue.pop();
                }

                _parseCSVFile(csvPath);
            }
        });
    }
}

void CSVParsingQueue::_enqueueParsingTask(const std::filesystem::path& csvFilePath)
{
    {
        std::unique_lock<std::mutex> lock{m_QueueMutex};
        m_CsvFilePathsQueue.emplace(csvFilePath);
    }

    m_QueueConditionVariable.notify_one();
}

void CSVParsingQueue::_parseCSVFile(const std::filesystem::path& inputCSVPath)
{
    if (Utils::isCsvFilePath(inputCSVPath))
    {
        CSVParser parser{inputCSVPath};
        parser.parse();
        m_CsvAggregator.storeHostData(parser.getOutput());
    }
}
