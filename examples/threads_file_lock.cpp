/*!
    \file threads_file_lock.cpp
    \brief File-lock synchronization primitive example
    \author Ivan Shynkarenka
    \date 01.09.2016
    \copyright MIT License
*/

#include "threads/file_lock.h"
#include "threads/thread.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    std::cout << "Press Enter to stop..." << std::endl;

    CppCommon::FileLock lock_master(".lock");

    int current = 0;
    std::atomic<bool> stop(false);

    // Start some producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < 4; ++producer)
    {
        producers.emplace_back([&stop, &current, producer]()
        {
            CppCommon::FileLock lock_slave(".lock");

            while (!stop)
            {
                // Use a write locker to produce the item
                {
                    CppCommon::WriteLocker<CppCommon::FileLock> locker(lock_slave);

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
        consumers.emplace_back([&stop, &current, consumer]()
        {
            CppCommon::FileLock lock_slave(".lock");

            while (!stop)
            {
                // Use a read locker to consume the item
                {
                    CppCommon::ReadLocker<CppCommon::FileLock> locker(lock_slave);

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
