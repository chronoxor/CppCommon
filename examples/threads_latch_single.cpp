/*!
    \file threads_latch_single.cpp
    \brief Latch synchronization primitive example for single threads waiting
    \author Ivan Shynkarenka
    \date 25.03.2016
    \copyright MIT License
*/

#include "threads/critical_section.h"
#include "threads/latch.h"
#include "threads/thread.h"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    int concurrency = 8;
    CppCommon::Latch latch(1);
    CppCommon::CriticalSection locker;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&latch, &locker, thread]()
        {
            locker.lock();
            std::cout << "Thread " << thread << " waiting for the latch..." << std::endl;
            locker.unlock();

            // Wait for the latch
            latch.Wait();

            locker.lock();
            std::cout << "Thread " << thread << " continue!" << std::endl;
            locker.unlock();

            // Sleep for a while...
            CppCommon::Thread::Sleep(thread * 100);
        }));
    }

    // Perform some initialization
    CppCommon::Thread::Sleep(100);

    locker.lock();
    std::cout << "Main thread initialized!" << std::endl;
    locker.unlock();

    // Threads can now start processing
    latch.CountDown();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
