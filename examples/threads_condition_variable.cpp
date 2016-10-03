/*!
    \file threads_condition_variable.cpp
    \brief Condition variable synchronization primitive example
    \author Ivan Shynkarenka
    \date 03.10.2016
    \copyright MIT License
*/

#include "threads/condition_variable.h"
#include "threads/thread.h"

#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    std::string help = "Please enter '+' to notify one waiting thread, enter '*' to notify all waiting threads of the condition variable...";

    // Show help message
    std::cout << help << std::endl;

    int concurrency = 8;

    // Condition variable sample
    bool finish = false;
    CppCommon::CriticalSection cs;
    CppCommon::ConditionVariable cv;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv, &finish, thread]()
        {
            std::cout << "Thread " << thread << " initialized!" << std::endl;

            // Sleep for a while...
            CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(thread * 10));

            std::cout << "Thread " << thread << " waiting for the notification!" << std::endl;

            // Lock the critical section
            cs.Lock();

            // Safe check for the finish flag under the lock
            while (!finish)
            {
                // Wait for the notification
                cv.Wait(cs);

                std::cout << "Thread " << thread << " notified!" << std::endl;
            }

            // Unlock the critical section
            cs.Unlock();

            std::cout << "Thread " << thread << " finished!" << std::endl;
        }));
    }

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "+")
        {
            std::cout << "Notify one thread!" << std::endl;
            cv.NotifyOne();
        }
        else if (line == "*")
        {
            // Lock the critical section
            cs.Lock();

            // Safe set the finish flag under the lock
            finish = true;

            // Unlock the critical section
            cs.Unlock();

            std::cout << "Notify all threads!" << std::endl;
            cv.NotifyAll();
            break;
        }
        else
            std::cout << help << std::endl;
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
