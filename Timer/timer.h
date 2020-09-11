/* Timer class created using multithreading and observer pattern:
   - each timer can act in active (timeout) and passive (elapsed) mode
   - each timer can have one or more observers (timeout handlers ITimeoutHandler) that are invoked when timeout occurs
   - the timer can be started, stopped or (re)started with same timeout interval as when previously run
   - timeout interval 0 triggers immediate timeout
   - timer runs in separate non-blocking thread in active mode
*/

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>
#include <vector>

using h_r_clock_t = std::chrono::high_resolution_clock;
using millisecond_t = std::chrono::duration<double, std::ratio<1, 1000> >;

class ITimeoutHandler;

class Timer
{
public:
    Timer(std::string m_Name = "");
    virtual ~Timer(); // in case the class is inherited for creating more complex timers

    void start(size_t duration = 0);
    void restart();
    void stop();
    void reset(); // for use as "passive" timer, i.e. when the timer is not running

    double getElapsedTime() const;
    double getTimeoutInterval() const;
    std::string getName() const;
    bool isRunning() const;

    void addTimeoutHandler(ITimeoutHandler* handler);
    void removeTimeoutHandler(ITimeoutHandler* handler);

protected:
    void timeout(); // in case inheriting is required for creating more complex timers

private:
    void _sampleForTimeout();
    h_r_clock_t::time_point m_StartTime;
    millisecond_t m_TimeoutInterval;
    bool m_IsRunning;
    std::string m_Name;
    std::vector<ITimeoutHandler*> m_TimeoutHandlers;
};

class ITimeoutHandler
{
public:
    virtual void onTimeout(const Timer* const timer) = 0;
    virtual ~ITimeoutHandler();
};

#endif // TIMER_H
