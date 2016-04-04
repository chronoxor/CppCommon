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
#include <mutex>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    CppCommon::Mutex locker;

    std::cout << "Press Enter to stop..." << std::endl;

    // Start some threads
    std::atomic<bool> stop(false);
    std::vector<std::thread> threads;
    for (int thread = 0; thread < 4; ++thread)
    {
        threads.push_back(std::thread([&locker, &stop, thread]()
        {
            while (!stop)
            {
                // Use lock guard with mutex to protect the output
                std::lock_guard<CppCommon::Mutex> lock(locker);

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
