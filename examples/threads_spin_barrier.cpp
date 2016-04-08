/*!
    \file threads_spin_barrier.cpp
    \brief Spin barrier synchronization primitive example
    \author Ivan Shynkarenka
    \date 17.03.2016
    \copyright MIT License
*/

#include "threads/critical_section.h"
#include "threads/spin_barrier.h"
#include "threads/thread.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    int concurrency = 8;
    CppCommon::SpinBarrier barrier(concurrency);
    CppCommon::CriticalSection lock;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&barrier, &lock, thread]()
        {
            lock.Lock();
            std::cout << "Thread " << thread << " initialized!" << std::endl;
            lock.Unlock();

            // Sleep for a while...
            CppCommon::Thread::SleepFor(std::chrono::milliseconds(thread * 100));

            lock.Lock();
            std::cout << "Thread " << thread << " before barrier!" << std::endl;
            lock.Unlock();

            // Wait for all other threads at the barrier
            bool last = barrier.Wait();

            lock.Lock();
            std::cout << "Thread " << thread << " after barrier!" << (last ? " Last one!" : "") << std::endl;
            lock.Unlock();
        }));
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
