//
// Created by Ivan Shynkarenka on 27.01.2016.
//

#include "catch.hpp"

#include <mutex>

#include "threads/critical_section.h"

using namespace CppCommon;

TEST_CASE("Critical section", "[CppCommon][Threads]")
{
    CriticalSection lock;

    // Test TryLock() method
    REQUIRE(lock.TryLock());
    lock.unlock();

    // Test TryLockSpin() method
    for (int i = -10; i < 10; ++i)
    {
        REQUIRE(lock.TryLockSpin(i));
        lock.unlock();
    }

    // Test lock()/unlock() methods
    lock.lock();
    lock.unlock();
}

TEST_CASE("Critical section lock guard", "[CppCommon][Threads]")
{
    CriticalSection lock;

    int items_to_produce = 1000000;
    int producers_count = 4;
    int crc = 0;

    // Caclulate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.push_back(std::thread([&lock, &crc, producer, items_to_produce, producers_count]()
        {
            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
            {
                std::lock_guard<CriticalSection> guard(lock);
                crc += (producer * items) + i;
            }
        }));
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check result
    REQUIRE(crc == result);
}
