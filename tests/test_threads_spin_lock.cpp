//
// Created by Ivan Shynkarenka on 22.01.2016
//

#include "test.h"

#include "threads/spin_lock.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Spin-lock", "[CppCommon][Threads]")
{
    SpinLock lock;

    // Test IsLocked() method
    REQUIRE(!lock.IsLocked());

    // Test TryLock() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.IsLocked());
    lock.Unlock();
    REQUIRE(!lock.IsLocked());

    // Test TryLockSpin() method
    for (int i = -10; i < 10; ++i)
    {
        REQUIRE(lock.TryLockSpin(i));
        REQUIRE(lock.IsLocked());
        REQUIRE(!lock.TryLockSpin(i));
        lock.Unlock();
        REQUIRE(!lock.IsLocked());
    }

    // Test TryLockFor() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.IsLocked());
    int64_t start = Timestamp::nano();
    REQUIRE(!lock.TryLockFor(Timespan::nanoseconds(100)));
    int64_t stop = Timestamp::nano();
    REQUIRE(((stop - start) >= 100));
    lock.Unlock();
    REQUIRE(!lock.IsLocked());

    // Test TryLockUntil() method
    REQUIRE(lock.TryLock());
    REQUIRE(lock.IsLocked());
    start = Timestamp::nano();
    REQUIRE(!lock.TryLockUntil(UtcTimestamp() + Timespan::nanoseconds(100)));
    stop = Timestamp::nano();
    REQUIRE(((stop - start) >= 100));
    lock.Unlock();
    REQUIRE(!lock.IsLocked());

    // Test Lock()/Unlock() methods
    REQUIRE(!lock.IsLocked());
    lock.Lock();
    REQUIRE(lock.IsLocked());
    lock.Unlock();
    REQUIRE(!lock.IsLocked());
}

TEST_CASE("Spin-lock locker", "[CppCommon][Threads]")
{
    int items_to_produce = 1000000;
    int producers_count = 4;
    int crc = 0;

    SpinLock lock;

    REQUIRE(!lock.IsLocked());

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
                Locker<SpinLock> locker(lock);
                crc += (producer * items) + i;
            }
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check result
    REQUIRE(crc == result);

    REQUIRE(!lock.IsLocked());
}
