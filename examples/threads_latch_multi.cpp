/*!
    \file threads_latch_multi.cpp
    \brief Latch synchronization primitive example for multiple threads waiting
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
    CppCommon::Latch latch(concurrency);
    CppCommon::CriticalSection lock;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&latch, &lock, thread]()
        {
            lock.Lock();
            std::cout << "Thread " << thread << " initialized!" << std::endl;
            lock.Unlock();

            // Sleep for a while...
            CppCommon::Thread::SleepFor(std::chrono::milliseconds(thread * 100));

            // Count down the latch
            latch.CountDown();

            lock.Lock();
            std::cout << "Thread " << thread << " latch count down!" << std::endl;
            lock.Unlock();
        }));
    }

    lock.Lock();
    std::cout << "Main thread is waiting for the latch..." << std::endl;
    lock.Unlock();

    // Wait until work is done
    latch.Wait();

    lock.Lock();
    std::cout << "Main thread continue!" << std::endl;
    lock.Unlock();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
