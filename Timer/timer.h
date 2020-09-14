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

/* single-shot timer */
class Timer
{
public:
    Timer(std::string name = "");
    virtual ~Timer(); // in case the class is inherited for creating more complex timers

    virtual void start(size_t duration = 0);
    virtual void restart();
    virtual void stop();

    void reset(); // for use as "passive" timer, i.e. when the timer is not running

    void setTimeoutInterval(size_t timeoutInterval);
    double getTimeoutInterval() const;

    double getElapsedTime() const;
    std::string getName() const;
    bool isRunning() const;

    void addTimeoutHandler(ITimeoutHandler* handler);
    void removeTimeoutHandler(ITimeoutHandler* handler);

protected:
    void timeout(); // in case inheriting is required for creating more complex timers
    void resetCurrentTime(); // enables resetting current time while timer running

    bool m_IsRunning;

private:
    void _doStart();
    void _sampleForTimeout();

    h_r_clock_t::time_point m_StartTime;
    millisecond_t m_TimeoutInterval;
    std::string m_Name;
    std::vector<ITimeoutHandler*> m_TimeoutHandlers;
};

/* repeating timer */
class CyclicalTimer final : public Timer
{
public:
    CyclicalTimer(std::string name = "");
    ~CyclicalTimer() override;

    void start(size_t duration = 0) override;
    void restart() override;

private:
    void _doStart();
    void _run();
};

class ITimeoutHandler
{
public:
    virtual void onTimeout(const Timer* const timer) = 0;
    virtual ~ITimeoutHandler();
};

#endif // TIMER_H
