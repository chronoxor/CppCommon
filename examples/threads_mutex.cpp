/*!
    \file threads_mutex.cpp
    \brief Mutex synchronization primitive example
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#include "threads/mutex.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    std::cout << "Press Enter to stop..." << std::endl;

    CppCommon::Mutex lock;

    // Start some threads
    std::atomic<bool> stop(false);
    std::vector<std::thread> threads;
    for (int thread = 0; thread < 4; ++thread)
    {
        threads.emplace_back([&lock, &stop, thread]()
        {
            while (!stop)
            {
                // Use locker with mutex to protect the output
                CppCommon::Locker<CppCommon::Mutex> locker(lock);

                std::cout << "Random value from thread " << thread << ": " << rand() << std::endl;
            }
        });
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
