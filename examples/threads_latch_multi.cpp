/*!
    \file threads_latch_multi.cpp
    \brief Latch synchronization primitive example for multiple threads waiting
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

    CppCommon::Latch latch(concurrency);

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&latch, thread]()
        {
            std::cout << "Thread " << thread << " initialized!" << std::endl;

            // Sleep for a while...
            CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(thread * 10));

            // Count down the latch
            latch.CountDown();

            std::cout << "Thread " << thread << " latch count down!" << std::endl;
        });
    }

    std::cout << "Main thread is waiting for the latch..." << std::endl;

    // Wait until work is done
    latch.Wait();

    std::cout << "Main thread continue!" << std::endl;

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
