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
    lock.try_lock();
    REQUIRE(lock.is_locked());
    lock.unlock();
    REQUIRE(!lock.is_locked());

    // Test try_lock_for() method
    for (int i = -10; i < 10; ++i)
    {
        lock.try_lock_for(i);
        REQUIRE(lock.is_locked());
        lock.unlock();
        REQUIRE(!lock.is_locked());
    }

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
