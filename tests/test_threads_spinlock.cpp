//
// Created by Ivan Shynkarenka on 22.01.2016.
//

#include "catch.hpp"

#include <mutex>

#include "threads/spinlock.h"

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

    // Test with std::lock_guard
    for (int i = 0; i < 10; ++i)
    {
        std::lock_guard<SpinLock> guard(lock);
        REQUIRE(lock.IsLocked());
    }

    REQUIRE(!lock.IsLocked());
}
