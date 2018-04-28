//
// Created by Ivan Shynkarenka on 02.09.2016
//

#include "test.h"

#include "threads/file_lock.h"
#include "threads/thread.h"

#include <thread>
#include <vector>

using namespace CppCommon;

TEST_CASE("File-lock", "[CppCommon][Threads]")
{
    FileLock lock1(".lock");
    FileLock lock2(".lock");

    // Test TryLockRead() method
    REQUIRE(lock1.TryLockRead());
    REQUIRE(!lock2.TryLockWrite());
    lock1.UnlockRead();

    // Test TryLockWrite() method
    REQUIRE(lock1.TryLockWrite());
    REQUIRE(!lock2.TryLockRead());
    lock1.UnlockWrite();

    // Test LockRead()/UnlockRead() methods
    lock1.LockRead();
    REQUIRE(!lock2.TryLockWrite());
    lock1.UnlockRead();

    // Test LockWrite()/UnlockWrite() methods
    lock1.LockWrite();
    REQUIRE(!lock2.TryLockRead());
    lock1.UnlockWrite();
}

TEST_CASE("File-locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10;
    int consumers_count = 4;
    int crc = 0;
    std::vector<int> crcs;
    int current = 0;

    FileLock lock_master(".lock");

    // Reset consumers' results
    for (int i = 0; i < consumers_count; ++i)
        crcs.push_back(0);

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producer thread
    std::thread producer = std::thread([&crc, &current, items_to_produce]()
    {
        FileLock lock_slave(".lock");

        for (int i = 0; i < items_to_produce; ++i)
        {
            // Use a write locker to produce the item
            {
                WriteLocker<FileLock> locker(lock_slave);

                // Update the current produced item and produced crc
                current = i;
                crc += current;
            }

            // Sleep for a while...
            Thread::Sleep(10);
        }
    });

    // Start consumers threads
    std::vector<std::thread> consumers;
    for (int consumer = 0; consumer < consumers_count; ++consumer)
    {
        consumers.emplace_back([&crcs, &current, consumer, items_to_produce]()
        {
            FileLock lock_slave(".lock");

            int item = 0;
            while (item < (items_to_produce - 1))
            {
                // Use a read locker to consume the item
                {
                    ReadLocker<FileLock> locker(lock_slave);

                    // Check for the current item changed
                    if (item != current)
                    {
                        // Update consumed crc
                        item = current;
                        crcs[consumer] += item;
                    }
                }

                // Yield to another thread...
                Thread::Yield();
            }
        });
    }

    // Wait for producer thread
    producer.join();

    // Wait for all consumers threads
    for (auto& consumer : consumers)
        consumer.join();

    // Check result
    REQUIRE(crc == result);
    for (int i = 0; i < consumers_count; ++i)
        REQUIRE(crcs[i] > 0);
}
