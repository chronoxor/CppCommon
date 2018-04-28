/*!
    \file threads_latch_single.cpp
    \brief Latch synchronization primitive example for single threads waiting
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#include "threads/latch.h"
#include "threads/thread.h"

#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    int concurrency = 8;

    CppCommon::Latch latch(1);

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&latch, thread]()
        {
            std::cout << "Thread " << thread << " waiting for the latch..." << std::endl;

            // Wait for the latch
            latch.Wait();

            std::cout << "Thread " << thread << " continue!" << std::endl;

            // Sleep for a while...
            CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(thread * 10));
        });
    }

    // Perform some initialization
    CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(100));

    std::cout << "Main thread initialized!" << std::endl;

    // Threads can now start processing
    latch.CountDown();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
