/*!
    \file threads_named_condition_variable.cpp
    \brief Named condition variable synchronization primitive example
    \author Ivan Shynkarenka
    \date 04.10.2016
    \copyright MIT License
*/

#include "threads/named_condition_variable.h"
#include "threads/thread.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    std::string help = "Please enter '+' to notify one waiting thread, enter '*' to notify all waiting threads of the condition variable...";

    // Show help message
    std::cout << help << std::endl;

    int concurrency = 8;
    std::atomic<bool> finish(false);

    // Named condition variable master
    CppCommon::NamedConditionVariable cv_master("named_cv_example");

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&finish, thread]()
        {
            // Named condition variable slave
            CppCommon::NamedConditionVariable cv_slave("named_cv_example");

            std::cout << "Thread " << thread << " initialized!" << std::endl;

            // Sleep for a while...
            CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(thread * 10));

            std::cout << "Thread " << thread << " waiting for the notification!" << std::endl;

            // Safe atomic check for the finish flag
            while (!finish)
            {
                // Wait for the notification
                cv_slave.Wait();

                std::cout << "Thread " << thread << " notified!" << std::endl;
            }

            std::cout << "Thread " << thread << " finished!" << std::endl;
        });
    }

    // Perform text input
    std::string line;
    while (!finish && getline(std::cin, line))
    {
        if (line == "+")
        {
            std::cout << "Notify one thread!" << std::endl;
            cv_master.NotifyOne();
        }
        else if (line == "*")
        {
            // Safe set the finish flag under the lock
            finish = true;

            std::cout << "Notify all threads!" << std::endl;
            cv_master.NotifyAll();
        }
        else
            std::cout << help << std::endl;
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
