//
// Created by Ivan Shynkarenka on 12.04.2016
//

#include "test.h"

#include "threads/semaphore.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Semaphore", "[CppCommon][Threads]")
{
    Semaphore lock(4);

    // Test TryLock() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.TryLock());
    REQUIRE(lock.TryLock());
    REQUIRE(lock.TryLock());
    REQUIRE(!lock.TryLock());
    lock.Unlock();
    lock.Unlock();
    lock.Unlock();
    lock.Unlock();

    // Test Lock()/Unlock() methods
    lock.Lock();
    lock.Lock();
    lock.Lock();
    lock.Lock();
    REQUIRE(!lock.TryLock());
    lock.Unlock();
    lock.Unlock();
    lock.Unlock();
    lock.Unlock();
    REQUIRE(lock.TryLock());
    lock.Unlock();
}

TEST_CASE("Semaphore locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 8;
    std::atomic<int> crc(0);

    Semaphore lock(4);

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&lock, &crc, producer, items_to_produce, producers_count]()
        {
            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
            {
                Locker<Semaphore> locker(lock);
                crc += (producer * items) + i;
            }
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check result
    REQUIRE(crc == result);
}
