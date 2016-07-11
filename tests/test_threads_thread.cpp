//
// Created by Ivan Shynkarenka on 27.01.2016.
//

#include "catch.hpp"

#include "threads/thread.h"
#include "time/timestamp.h"

using namespace CppCommon;

TEST_CASE("Thread", "[CppCommon][Threads]")
{
    REQUIRE(Thread::CurrentThreadId() > 0);

    // Test Sleep() method
    for (int64_t i = 1; i < 1000000; i *= 10)
    {
        int64_t start = timestamp();
        Thread::Sleep(i);
        int64_t stop = timestamp();
        REQUIRE(((stop - start) >= 0));
    }

    // Test SleepFor() method
    for (int64_t i = 1; i < 1000000; i *= 10)
    {
        int64_t start = timestamp();
        Thread::SleepFor(std::chrono::nanoseconds(i));
        int64_t stop = timestamp();
        REQUIRE(((stop - start) >= 0));
    }

    // Test SleepUntil() method
    for (int64_t i = 1; i < 1000000; i *= 10)
    {
        int64_t start = timestamp();
        Thread::SleepUntil(std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(i));
        int64_t stop = timestamp();
        REQUIRE(((stop - start) >= 0));
    }

    // Test Yield() method
    for (int64_t i = 0; i < 10; ++i)
    {
        int64_t start = timestamp();
        Thread::Yield();
        int64_t stop = timestamp();
        REQUIRE(((stop - start) >= 0));
    }
}
