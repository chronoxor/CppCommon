/*!
    \file threads_rw_lock.cpp
    \brief Read/Write lock synchronization primitive example
    \author Ivan Shynkarenka
    \date 08.04.2016
    \copyright MIT License
*/

#include "threads/rw_lock.h"
#include "threads/thread.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    std::cout << "Press Enter to stop..." << std::endl;

    CppCommon::RWLock lock;

    int current = 0;
    std::atomic<bool> stop(false);

    // Start some producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < 4; ++producer)
    {
        producers.emplace_back([&lock, &stop, &current, producer]()
        {
            while (!stop)
            {
                // Use a write locker to produce the item
                {
                    CppCommon::WriteLocker<CppCommon::RWLock> locker(lock);

                    current = rand();
                    std::cout << "Produce value from thread " << producer << ": " << current << std::endl;
                }

                // Sleep for a while...
                CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds((producer + 1) * 1000));
            }
        });
    }

    // Start some consumers threads
    std::vector<std::thread> consumers;
    for (int consumer = 0; consumer < 4; ++consumer)
    {
        consumers.emplace_back([&lock, &stop, &current, consumer]()
        {
            while (!stop)
            {
                // Use a read locker to consume the item
                {
                    CppCommon::ReadLocker<CppCommon::RWLock> locker(lock);

                    std::cout << "Consume value in thread " << consumer << ": " << current << std::endl;
                }

                // Sleep for a while...
                CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(100));
            }
        });
    }

    // Wait for input
    std::cin.get();

    // Stop threads
    stop = true;

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Wait for all consumers threads
    for (auto& consumer : consumers)
        consumer.join();

    return 0;
}
