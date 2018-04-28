/*!
    \file threads_named_event_auto_reset.cpp
    \brief Named auto-reset event synchronization primitive example
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#include "threads/named_event_auto_reset.h"
#include "threads/thread.h"

#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    std::string help = "Please enter '!' to signal the named auto-reset event (several processes support). Enter '0' to exit...";

    // Show help message
    std::cout << help << std::endl;

    int concurrency = 8;

    // Named auto-reset event master
    CppCommon::NamedEventAutoReset event_master("named_auto_event_example");

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([thread]()
        {
            // Named auto-reset event slave
            CppCommon::NamedEventAutoReset event_slave("named_auto_event_example");

            std::cout << "Thread " << thread << " initialized!" << std::endl;

            // Sleep for a while...
            CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(thread * 10));

            std::cout << "Thread " << thread << " waiting for the event!" << std::endl;

            // Wait for the event
            event_slave.Wait();

            std::cout << "Thread " << thread << " signaled!" << std::endl;
        });
    }

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "!")
        {
            std::cout << "Signal event!" << std::endl;
            event_master.Signal();
        }
        else if (line == "0")
            break;
        else
            std::cout << help << std::endl;
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
