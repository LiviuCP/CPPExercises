#include <iostream>
#include <thread>
#include <cassert>

#include "timer.h"

using namespace std;

class SimpleObserver: public ITimeoutHandler
{
public:
    SimpleObserver();
    void setTimer(Timer* timer);
    void onTimeout(const Timer* const timer);
private:
    const Timer* m_Timer1;
};

class ComplexObserver : public ITimeoutHandler
{
public:
    ComplexObserver();
    void setFirstTimer(const Timer* timer);
    void setSecondTimer(const Timer* timer);
    void onTimeout(const Timer* const timer);
private:
    const Timer* m_Timer1;
    const Timer* m_Timer2;
};

int main()
{
    SimpleObserver simpleObserver;
    ComplexObserver complexObserver;

    Timer timer1{"T1"}, timer2{"T2"}, timer3{"T3"}, timer4{"T4"}, timer5{"T5"};

    complexObserver.setFirstTimer(&timer1);
    complexObserver.setSecondTimer(&timer2);
    simpleObserver.setTimer(&timer1);

    timer1.addTimeoutHandler(&complexObserver);
    timer1.addTimeoutHandler(&simpleObserver);
    timer2.addTimeoutHandler(&complexObserver);
    timer3.addTimeoutHandler(&complexObserver);
    timer4.addTimeoutHandler(&simpleObserver);
    timer5.addTimeoutHandler(&complexObserver);

    cout << "* Starting all timers" << endl;

    timer1.start(5000);
    timer2.start(3500);
    timer3.start(2500);
    timer4.start(7000);
    timer5.start(9000);

    while (timer1.isRunning() || timer2.isRunning() || timer3.isRunning() || timer4.isRunning());

    cout << "* First four timers timed out" << endl;
    if (timer5.isRunning())
    {
        cout << "* Stopping timer " << timer5.getName() << endl;
        timer5.stop();
    }

    this_thread::sleep_for(millisecond_t{500});
    cout << endl << "* Restarting timer " << timer3.getName() << " and starting timer " << timer4.getName() << " with a different timeout period" << endl;
    timer3.restart();
    timer4.start(4500);
    while (timer3.isRunning() || timer4.isRunning());
    cout << "* Both timers timed out" << endl;

    this_thread::sleep_for(millisecond_t{500});
    cout << endl << "* Restarting timer " << timer2.getName() << " for immediate timeout" << endl;
    timer2.start();
    while (timer2.isRunning());
    cout << "* Timer timed out" << endl;

    this_thread::sleep_for(millisecond_t{500});
    cout << endl << "* Removing simple object from timeout handlers of " << timer1.getName() << " and adding it to timer " << timer3.getName() << endl;
    timer1.removeTimeoutHandler(&simpleObserver);
    timer3.addTimeoutHandler(&simpleObserver);
    cout << "* Adding timer " << timer5.getName() << " as internal timer of simple object and adding object to handlers for this timer" << endl;
    timer5.addTimeoutHandler(&simpleObserver);
    simpleObserver.setTimer(&timer5);
    cout << "* Restarting all 3 timers with same timout intervals as before namely: " << timer1.getTimeoutInterval() << ", " << timer3.getTimeoutInterval() << " and " << timer5.getTimeoutInterval();
    cout << " milliseconds" << endl;
    timer1.restart();
    timer3.restart();
    timer5.restart();
    while (timer1.isRunning() || timer3.isRunning() || timer5.isRunning());
    cout << "* All remaining timers timed out. Goodbye!" << endl << endl;

    return 0;
}

SimpleObserver::SimpleObserver()
    : m_Timer1{nullptr}
{
}

void SimpleObserver::setTimer(Timer* timer)
{
    if (timer)
    {
        m_Timer1 = timer;
    }
}

void SimpleObserver::onTimeout(const Timer * const timer)
{
    assert(timer && "Null pointer detected for timer");

    if (m_Timer1 == timer)
    {
        cout << "SimpleObserver, internal timer " << timer->getName() << " expired" << endl;
    }
    else
    {
        cout << "SimpleObserver, external timer " << timer->getName() << " expired" << endl;
    }
}

ComplexObserver::ComplexObserver()
    : m_Timer1{nullptr}
    , m_Timer2{nullptr}
{
}

void ComplexObserver::setFirstTimer(const Timer* timer)
{
    if (timer)
    {
        m_Timer1 = timer;
    }
}

void ComplexObserver::setSecondTimer(const Timer* timer)
{
    if (timer)
    {
        m_Timer2 = timer;
    }
}

void ComplexObserver::onTimeout(const Timer * const timer)
{
    assert(timer && "Null pointer detected for timer");

    if (m_Timer1 == timer)
    {
        cout << "ComplexObserver, first internal timer " << timer->getName() << " expired" << endl;
    }
    else if (m_Timer2 == timer)
    {
        cout << "ComplexObserver, second internal timer " << timer->getName() << " expired" << endl;
    }
    else
    {
        cout << "ComplexObserver, external timer " << timer->getName() << " expired" << endl;
    }
}
