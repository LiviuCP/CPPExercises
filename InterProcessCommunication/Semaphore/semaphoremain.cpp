#include <iostream>
#include <string>
#include <thread>

#include "semaphore.h"
#include "utils.h"

void useResource(const std::string& threadName, int milliseconds, Semaphore& semaphore);

int main(int argc, char** argv)
{
    Utilities::clearScreen();

    const int c_ConcurrencyFactor{argc == 1 ? 0 : atoi(argv[1])}; // number of concurrent threads allowed by semaphore
    Semaphore semaphore{c_ConcurrencyFactor};

    std::cout << "Created a semaphore" << std::endl;

    if (!semaphore.isLocked())
    {
        std::cout << "The semaphore is available, we will now create the threads" << std::endl << std::endl;

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

        std::cout << std::endl << "All threads ended. ";
    }
    else
    {
        std::cout << "Semaphore is locked. Cannot create any threads" << std::endl;
    }

    std::cout << "Semaphore is being destroyed" << std::endl;

    return 0;
}

void useResource(const std::string& threadName, int milliseconds, Semaphore& semaphore)
{
    semaphore.aquire(threadName);
    std::cout << "I am a resource, I am being used by thread " << threadName << " for " << milliseconds
              << " milliseconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{milliseconds});
    semaphore.release(threadName);
}
