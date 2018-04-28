//
// Created by Ivan Shynkarenka on 08.04.2016
//

#include "test.h"

#include "threads/rw_lock.h"
#include "threads/thread.h"

#include <thread>
#include <vector>

using namespace CppCommon;

TEST_CASE("Read/Write lock", "[CppCommon][Threads]")
{
    RWLock lock;

    // Test TryLockRead() method
    REQUIRE(lock.TryLockRead());
    REQUIRE(!lock.TryLockWrite());
    lock.UnlockRead();

    // Test TryLockWrite() method
    REQUIRE(lock.TryLockWrite());
    REQUIRE(!lock.TryLockRead());
    lock.UnlockWrite();

    // Test LockRead()/UnlockRead() methods
    lock.LockRead();
    REQUIRE(!lock.TryLockWrite());
    lock.UnlockRead();

    // Test LockWrite()/UnlockWrite() methods
    lock.LockWrite();
    REQUIRE(!lock.TryLockRead());
    lock.UnlockWrite();
}

TEST_CASE("Read/Write locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10;
    int consumers_count = 4;
    int crc = 0;
    std::vector<int> crcs;
    int current = 0;

    RWLock lock;

    // Reset consumers' results
    for (int i = 0; i < consumers_count; ++i)
        crcs.push_back(0);

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producer thread
    std::thread producer = std::thread([&lock, &crc, &current, items_to_produce]()
    {
        for (int i = 0; i < items_to_produce; ++i)
        {
            // Use a write locker to produce the item
            {
                WriteLocker<RWLock> locker(lock);

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
        consumers.emplace_back([&lock, &crcs, &current, consumer, items_to_produce]()
        {
            int item = 0;
            while (item < (items_to_produce - 1))
            {
                // Use a read locker to consume the item
                {
                    ReadLocker<RWLock> locker(lock);

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
