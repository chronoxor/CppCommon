//
// Created by Ivan Shynkarenka on 22.01.2016.
//

#include "catch.hpp"

#include "threads/spin_lock.h"

#include <mutex>
#include <thread>

using namespace CppCommon;

TEST_CASE("Spin-lock", "[CppCommon][Threads]")
{
    SpinLock lock;

    REQUIRE(!lock.IsLocked());

    // Test TryLock() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.IsLocked());
    lock.unlock();
    REQUIRE(!lock.IsLocked());

    // Test TryLockSpin() method
    for (int i = -10; i < 10; ++i)
    {
        REQUIRE(lock.TryLockSpin(i));
        REQUIRE(lock.IsLocked());
        REQUIRE(!lock.TryLockSpin(i));
        lock.unlock();
        REQUIRE(!lock.IsLocked());
    }

    // Test TryLockFor() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.IsLocked());
    int64_t start = timestamp();
    REQUIRE(!lock.TryLockFor(std::chrono::nanoseconds(100)));
    int64_t stop = timestamp();
    REQUIRE(((stop - start) >= 100));
    lock.unlock();
    REQUIRE(!lock.IsLocked());

    // Test TryLockUntil() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.IsLocked());
    start = timestamp();
    REQUIRE(!lock.TryLockUntil(std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(100)));
    stop = timestamp();
    REQUIRE(((stop - start) >= 100));
    lock.unlock();
    REQUIRE(!lock.IsLocked());

    // Test lock()/unlock() methods
    REQUIRE(!lock.IsLocked());
    lock.lock();
    REQUIRE(lock.IsLocked());
    lock.unlock();
    REQUIRE(!lock.IsLocked());
}

TEST_CASE("Spin-lock guard", "[CppCommon][Threads]")
{
    SpinLock lock;

    REQUIRE(!lock.IsLocked());

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
                std::lock_guard<SpinLock> guard(lock);
                crc += (producer * items) + i;
            }
        }));
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check result
    REQUIRE(crc == result);

    REQUIRE(!lock.IsLocked());
}
