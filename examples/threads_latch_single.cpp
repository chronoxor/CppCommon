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
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    int concurrency = 8;
    CppCommon::Latch latch(1);
    CppCommon::CriticalSection lock;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&latch, &lock, thread]()
        {
            lock.Lock();
            std::cout << "Thread " << thread << " waiting for the latch..." << std::endl;
            lock.Unlock();

            // Wait for the latch
            latch.Wait();

            lock.Lock();
            std::cout << "Thread " << thread << " continue!" << std::endl;
            lock.Unlock();

            // Sleep for a while...
            CppCommon::Thread::Sleep(thread * 100);
        }));
    }

    // Perform some initialization
    CppCommon::Thread::Sleep(100);

    lock.Lock();
    std::cout << "Main thread initialized!" << std::endl;
    lock.Unlock();

    // Threads can now start processing
    latch.CountDown();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
