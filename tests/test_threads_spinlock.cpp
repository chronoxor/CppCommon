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

    REQUIRE(!lock.is_locked());

    // Test try_lock() method
    REQUIRE(lock.try_lock());
    REQUIRE(lock.is_locked());
    lock.unlock();
    REQUIRE(!lock.is_locked());

    // Test try_lock_spin() method
    for (int i = -10; i < 10; ++i)
    {
        REQUIRE(lock.try_lock_spin(i));
        REQUIRE(lock.is_locked());
        REQUIRE(!lock.try_lock_spin(i));
        lock.unlock();
        REQUIRE(!lock.is_locked());
    }

    // Test try_lock_for() method
    REQUIRE(lock.try_lock());
    REQUIRE(lock.is_locked());
    int64_t start = timestamp();
    REQUIRE(!lock.try_lock_for(std::chrono::nanoseconds(100)));
    int64_t stop = timestamp();
    REQUIRE(((stop - start) >= 100));
    lock.unlock();
    REQUIRE(!lock.is_locked());

    // Test try_lock_until() method
    REQUIRE(lock.try_lock());
    REQUIRE(lock.is_locked());
    start = timestamp();
    REQUIRE(!lock.try_lock_until(std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(100)));
    stop = timestamp();
    REQUIRE(((stop - start) >= 100));
    lock.unlock();
    REQUIRE(!lock.is_locked());

    // Test lock()/unlock() methods
    REQUIRE(!lock.is_locked());
    lock.lock();
    REQUIRE(lock.is_locked());
    lock.unlock();
    REQUIRE(!lock.is_locked());

    // Test with std::lock_guard
    for (int i = 0; i < 10; ++i)
    {
        std::lock_guard<SpinLock> guard(lock);
        REQUIRE(lock.is_locked());
    }

    REQUIRE(!lock.is_locked());
}
