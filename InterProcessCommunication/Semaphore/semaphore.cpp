#include <cassert>
#include <iostream>

#include "semaphore.h"

Semaphore::Semaphore(int count)
    : m_Count{count}
{
    assert(m_Count >= 0 && "Semaphore initialized with wrong value");
}

void Semaphore::aquire(const std::string& threadName)
{
    std::unique_lock<std::mutex> lock{m_Mutex};

    if (m_Count == 0)
    {
        std::clog << "Semaphore is locked, thread " << threadName << " needs to wait for release" << std::endl;
        m_CondVar.wait(lock, [this]() { return m_Count > 0; });
    }

    --m_Count;
    std::clog << "Semaphore aquired by thread " << threadName << std::endl;
}

void Semaphore::release(const std::string& threadName)
{
    std::unique_lock<std::mutex> lock{m_Mutex};
    ++m_Count;
    m_CondVar.notify_one();
    std::clog << "Semaphore released by thread " << threadName << std::endl;
}

bool Semaphore::isLocked() const
{
    return m_Count == 0;
}
