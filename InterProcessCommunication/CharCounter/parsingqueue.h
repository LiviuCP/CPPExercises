#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <cstdlib>

class Parser;

class ParsingQueue
{
public:
    ParsingQueue(size_t threadsCount = std::thread::hardware_concurrency());
    ~ParsingQueue();

    bool addParsingTasks(std::vector<Parser*>& parsers);

    // all files from the queue get parsed, then the threads are stopped
    void finishParsingAndStop();

    // stops threads once all currently running jobs are done, doesn't wait for the queue to get empty
    void stop() noexcept;

private:
    void _createParsingThreads();
    void _enqueueParsingTask(Parser* pParser);

    std::vector<std::thread> m_ThreadPool;
    std::queue<Parser*> m_QueuedParsers;
    std::mutex m_QueueMutex;
    std::condition_variable m_QueueConditionVariable;
    size_t m_ThreadsCount;
    bool m_ShouldStopParsing;
};
