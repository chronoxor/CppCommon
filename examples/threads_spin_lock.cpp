/*!
    \file threads_spin_lock.cpp
    \brief Spin-lock synchronization primitive example
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "threads/spin_lock.h"

int main(int argc, char** argv)
{
    CppCommon::SpinLock locker;

    std::cout << "Press Enter to stop..." << std::endl;

    // Start some threads
    std::atomic<bool> stop(false);
    std::vector<std::thread> threads;
    for (int thread = 0; thread < 4; ++thread)
    {
        threads.push_back(std::thread([thread, &locker, &stop]()
        {
            while (!stop)
            {
                // Use lock guard with spin-lock to protect the output
                std::lock_guard<CppCommon::SpinLock> lock(locker);

                std::cout << "Random value from thread " << thread << ": " << rand() << std::endl;
            }
        }));
    }

    // Wait for input
    std::cin.get();

    // Stop threads
    stop = true;

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
