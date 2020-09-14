#include <thread>
#include <algorithm>

#include "timer.h"

static constexpr double c_SamplingPeriod{0.5};

ITimeoutHandler::~ITimeoutHandler()
{
}

Timer::Timer(std::string name)
    : m_IsRunning{false}
    , m_StartTime{h_r_clock_t::now()}
    , m_TimeoutInterval{millisecond_t{0.0}}
    , m_Name{name}
{
}

Timer::~Timer()
{
    stop();
}

void Timer::start(size_t timeoutInterval)
{
    if (!m_IsRunning)
    {
        m_TimeoutInterval = millisecond_t{static_cast<double>(timeoutInterval)};
        _doStart();
    }
}

// start with pre-defined timeout interval (default is 0 - immediate timeout (unless changed in the meantime)
void Timer::restart()
{
    if (!m_IsRunning)
    {
        _doStart();
    }
}

void Timer::stop()
{
    if (m_IsRunning)
    {
        m_StartTime = h_r_clock_t::now();
        m_IsRunning = false;
        std::this_thread::sleep_for(millisecond_t{1.5 * c_SamplingPeriod});
    }
}

void Timer::reset()
{
    if (!m_IsRunning)
    {
        resetCurrentTime();
    }
}

void Timer::setTimeoutInterval(size_t timeoutInterval)
{
    if (!m_IsRunning)
    {
        m_TimeoutInterval = millisecond_t{static_cast<double>(timeoutInterval)};
    }
}

double Timer::getTimeoutInterval() const
{
    const double c_TimeoutInterval{m_TimeoutInterval.count()};
    return c_TimeoutInterval;
}

double Timer::getElapsedTime() const
{
    const double c_Elapsed{std::chrono::duration_cast<millisecond_t>(h_r_clock_t::now() - m_StartTime).count()};
    return c_Elapsed;
}

std::string Timer::getName() const
{
    return m_Name;
}

bool Timer::isRunning() const
{
    return m_IsRunning;
}

void Timer::addTimeoutHandler(ITimeoutHandler* handler)
{
    if (handler && (std::find(m_TimeoutHandlers.cbegin(), m_TimeoutHandlers.cend(), handler) == m_TimeoutHandlers.end()))
    {
        m_TimeoutHandlers.push_back(handler);
    }
}

void Timer::removeTimeoutHandler(ITimeoutHandler* handler)
{
    if (handler && (std::find(m_TimeoutHandlers.cbegin(), m_TimeoutHandlers.cend(), handler) != m_TimeoutHandlers.end()))
    {
        m_TimeoutHandlers.erase(std::remove(m_TimeoutHandlers.begin(), m_TimeoutHandlers.end(), handler)); // erase-remove idiom
    }
}

void Timer::timeout()
{
    m_IsRunning = false;
    for (auto* handler : m_TimeoutHandlers)
    {
        handler->onTimeout(this);
    }
}

void Timer::resetCurrentTime()
{
    m_StartTime = h_r_clock_t::now();
}

void Timer::_doStart()
{
    m_StartTime = h_r_clock_t::now();

    if (m_TimeoutInterval < millisecond_t{1.0})
    {
        timeout();
    }
    else
    {
        m_IsRunning = true;
        // a detachable thread is preferred to std::async because of the blocking destructor of the latter one
        std::thread samplingThread{&Timer::_sampleForTimeout, this};
        samplingThread.detach();
    }
}

void Timer::_sampleForTimeout()
{
    while(m_IsRunning)
    {
        if (getElapsedTime() < m_TimeoutInterval.count())
        {
            std::this_thread::sleep_for(millisecond_t{c_SamplingPeriod});
        }
        else
        {
            timeout();
            break;
        }
    }
}

CyclicalTimer::CyclicalTimer(std::string name)
    : Timer(name)
{
}

CyclicalTimer::~CyclicalTimer()
{
    stop();
}

void CyclicalTimer::start(size_t timeoutInterval)
{
    if (!m_IsRunning)
    {
        setTimeoutInterval(timeoutInterval);
        _doStart();
    }
}

void CyclicalTimer::restart()
{
    if (!m_IsRunning)
    {
        _doStart();
    }
}

void CyclicalTimer::_doStart()
{
    resetCurrentTime();
    if (getTimeoutInterval() >= 1.0)
    {
        m_IsRunning = true;
        // a detachable thread is preferred to std::async because of the blocking destructor of the latter one
        std::thread samplingThread{&CyclicalTimer::_run, this};
        samplingThread.detach();
    }
    else
    {
        timeout();
    }
}

void CyclicalTimer::_run()
{
    bool isTimedOut{false};
    while(m_IsRunning)
    {
        if (isTimedOut)
        {
            resetCurrentTime();
            isTimedOut = false;
        }
        else
        {
            if (getElapsedTime() < getTimeoutInterval())
            {
                std::this_thread::sleep_for(millisecond_t{c_SamplingPeriod});
            }
            else
            {
                isTimedOut = true;
                timeout();
                m_IsRunning = true;
            }
        }
    }
}
