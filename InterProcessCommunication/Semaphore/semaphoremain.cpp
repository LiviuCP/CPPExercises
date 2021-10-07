#include <iostream>
#include <string>
#include <thread>

#include "semaphore.h"
#include "utils.h"

using namespace std;

void useResource(const string& threadName, int milliseconds, Semaphore& semaphore);

int main(int argc, char** argv)
{
    Utilities::clearScreen();

    const int c_ConcurrencyFactor{argc == 1 ? 0 : atoi(argv[1])}; // number of concurrent threads allowed by semaphore
    Semaphore semaphore{c_ConcurrencyFactor};

    cout << "Created a semaphore" << endl;

    if (!semaphore.isLocked())
    {
        cout << "The semaphore is available, we will now create the threads" << endl << endl;

        std::thread firstThread{&useResource, "T1", 2000, std::ref(semaphore)};
        std::thread secondThread{&useResource, "T2", 1000, std::ref(semaphore)};
        std::thread thirdThread{&useResource, "T3", 6000, std::ref(semaphore)};
        std::thread fourthThread{&useResource, "T4", 1000, std::ref(semaphore)};
        std::thread fifthThread{&useResource, "T5", 4000, std::ref(semaphore)};

        firstThread.join();
        secondThread.join();
        thirdThread.join();
        fourthThread.join();
        fifthThread.join();

        cout << endl << "All threads ended. ";
    }
    else
    {
        cout << "Semaphore is locked. Cannot create any threads" << endl;
    }

    cout << "Semaphore is being destroyed" << endl;

    return 0;
}

void useResource(const string& threadName, int milliseconds, Semaphore& semaphore)
{
    semaphore.aquire(threadName);
    cout << "I am a resource, I am being used by thread " << threadName << " for " << milliseconds << " milliseconds" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{milliseconds});
    semaphore.release(threadName);
}
