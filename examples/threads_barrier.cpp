/*!
    \file threads_barrier.cpp
    \brief Barrier synchronization primitive example
    \author Ivan Shynkarenka
    \date 17.03.2016
    \copyright MIT License
*/

#include "threads/barrier.h"
#include "threads/thread.h"

#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    int concurrency = 8;

    CppCommon::Barrier barrier(concurrency);

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&barrier, thread]()
        {
            std::cout << "Thread " << thread << " initialized!" << std::endl;

            // Sleep for a while...
            CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(thread * 10));

            std::cout << "Thread " << thread << " before barrier!" << std::endl;

            // Wait for all other threads at the barrier
            bool last = barrier.Wait();

            std::cout << "Thread " << thread << " after barrier!" << (last ? " Last one!" : "") << std::endl;
        });
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
