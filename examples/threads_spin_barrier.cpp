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
#include <mutex>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    int concurrency = 8;
    CppCommon::SpinBarrier barrier(concurrency);
    CppCommon::CriticalSection locker;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&barrier, &locker, thread]()
        {
            locker.lock();
            std::cout << "Thread " << thread << " initialized!" << std::endl;
            locker.unlock();

            // Sleep for a while...
            CppCommon::Thread::Sleep(thread * 100);

            locker.lock();
            std::cout << "Thread " << thread << " before barrier!" << std::endl;
            locker.unlock();

            // Wait for all other threads at the barrier
            bool last = barrier.wait();

            locker.lock();
            std::cout << "Thread " << thread << " after barrier!" << (last ? " Last one!" : "") << std::endl;
            locker.unlock();
        }));
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
