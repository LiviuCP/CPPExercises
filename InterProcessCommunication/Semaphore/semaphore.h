#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>
#include <string>

class Semaphore
{
public:
    Semaphore(int count = 0);

    void aquire(const std::string& threadName);
    void release(const std::string& threadName);

    bool isLocked() const;

private:
    int m_Count;
    std::mutex m_Mutex;
    std::condition_variable m_CondVar;
};

#endif // SEMAPHORE_H
