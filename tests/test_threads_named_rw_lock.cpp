//
// Created by Ivan Shynkarenka on 25.05.2016
//

#include "test.h"

#include "threads/named_rw_lock.h"
#include "threads/thread.h"

#include <thread>
#include <vector>

using namespace CppCommon;

TEST_CASE("Named read/write lock", "[CppCommon][Threads]")
{
    // Named read/write lock master
    NamedRWLock master("named_rw_lock_test");

    // Test TryLockRead() method
    {
        NamedRWLock slave("named_rw_lock_test");

        REQUIRE(slave.TryLockRead());
        REQUIRE(!slave.TryLockWrite());
        slave.UnlockRead();
    }

    // Test TryLockWrite() method
    {
        NamedRWLock slave("named_rw_lock_test");

        REQUIRE(slave.TryLockWrite());
        REQUIRE(!slave.TryLockRead());
        slave.UnlockWrite();
    }

    // Test LockRead()/UnlockRead() methods
    {
        NamedRWLock slave("named_rw_lock_test");

        slave.LockRead();
        REQUIRE(!slave.TryLockWrite());
        slave.UnlockRead();
    }

    // Test LockWrite()/UnlockWrite() methods
    {
        NamedRWLock slave("named_rw_lock_test");

        slave.LockWrite();
        REQUIRE(!slave.TryLockRead());
        slave.UnlockWrite();
    }
}

TEST_CASE("Named read/write locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10;
    int consumers_count = 4;
    int crc = 0;
    std::vector<int> crcs;
    int current = 0;

    // Named read/write lock master
    NamedRWLock lock_master("named_rw_lock_test");

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
        // Named read/write lock slave
        NamedRWLock lock_slave("named_rw_lock_test");

        for (int i = 0; i < items_to_produce; ++i)
        {
            // Use a write locker to produce the item
            {
                WriteLocker<NamedRWLock> locker(lock_slave);

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
            // Named read/write lock slave
            NamedRWLock lock_slave("named_rw_lock_test");

            int item = 0;
            while (item < (items_to_produce - 1))
            {
                // Use a read locker to consume the item
                {
                    ReadLocker<NamedRWLock> locker(lock_slave);

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
