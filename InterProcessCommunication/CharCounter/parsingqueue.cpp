#include <cassert>
#include <chrono>
#include <iostream>

#include "parser.h"
#include "parsingqueue.h"

ParsingQueue::ParsingQueue(size_t threadsCount)
    : m_ThreadsCount{threadsCount}
    , m_ShouldStopParsing{false}
{
    assert(threadsCount > 0);
    bool success{true};

    try
    {
        _createParsingThreads();
    }
    catch (std::system_error&)
    {
        success = false;
        std::cerr << "An error occurred! Unable to start parsing\n";
    }

    if (!success)
    {
        stop();
    }
}

ParsingQueue::~ParsingQueue()
{
    stop();
}

bool ParsingQueue::addParsingTasks(std::vector<Parser*>& parsers)
{
    if (!m_ShouldStopParsing)
    {
        for (const auto& pParser : parsers)
        {
            if (pParser)
            {
                _enqueueParsingTask(pParser);
            }
        }
    }

    return !m_ShouldStopParsing;
}

void ParsingQueue::finishParsingAndStop()
{
    if (!m_ShouldStopParsing)
    {
        while (!m_QueuedParsers.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }

        stop();
    }
}

void ParsingQueue::stop() noexcept
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
        catch (std::system_error&)
        {
            // no action required, just catch the exception (should not propagate outside this method as it is also
            // called by destructor)
        }
    }
}

void ParsingQueue::_createParsingThreads()
{
    for (size_t threadNumber{0}; threadNumber < m_ThreadsCount; ++threadNumber)
    {
        m_ThreadPool.emplace_back([this]() {
            for (;;)
            {
                Parser* pParser{nullptr};

                {
                    std::unique_lock<std::mutex> lock{m_QueueMutex};
                    m_QueueConditionVariable.wait(lock,
                                                  [this]() { return !m_QueuedParsers.empty() || m_ShouldStopParsing; });

                    if (m_QueuedParsers.empty() || m_ShouldStopParsing)
                    {
                        break;
                    }

                    pParser = std::move(m_QueuedParsers.front());
                    m_QueuedParsers.pop();
                }

                if (pParser)
                {
                    pParser->parse();
                }
            }
        });
    }
}

void ParsingQueue::_enqueueParsingTask(Parser* pParser)
{
    {
        std::unique_lock<std::mutex> lock{m_QueueMutex};
        m_QueuedParsers.emplace(pParser);
    }

    m_QueueConditionVariable.notify_one();
}
