//
// Created by Ivan Shynkarenka on 27.01.2016
//

#include "test.h"

#include "threads/thread.h"
#include "time/timestamp.h"

using namespace CppCommon;

TEST_CASE("Thread", "[CppCommon][Threads]")
{
    REQUIRE(Thread::CurrentThreadId() > 0);
    REQUIRE(Thread::CurrentThreadAffinity() >= 0);

    // Test Sleep() method
    for (int64_t i = 1; i < 10; ++i)
    {
        int64_t start = Timestamp::nano();
        Thread::Sleep(i);
        int64_t stop = Timestamp::nano();
        REQUIRE(((stop - start) >= 0));
    }

    // Test SleepFor() method
    for (int64_t i = 1; i < 1000000; i *= 10)
    {
        int64_t start = Timestamp::nano();
        Thread::SleepFor(Timespan::nanoseconds(i));
        int64_t stop = Timestamp::nano();
        REQUIRE(((stop - start) >= 0));
    }

    // Test SleepUntil() method
    for (int64_t i = 1; i < 1000000; i *= 10)
    {
        int64_t start = Timestamp::nano();
        Thread::SleepUntil(UtcTimestamp() + Timespan::nanoseconds(i));
        int64_t stop = Timestamp::nano();
        REQUIRE(((stop - start) >= 0));
    }

    // Test Yield() method
    for (int64_t i = 0; i < 10; ++i)
    {
        int64_t start = Timestamp::nano();
        Thread::Yield();
        int64_t stop = Timestamp::nano();
        REQUIRE(((stop - start) >= 0));
    }

    // Test thread CPU affinity
    std::bitset<64> affinity = Thread::GetAffinity();
    REQUIRE(affinity.to_ullong() > 0);

    // Test thread priority
    ThreadPriority priority = Thread::GetPriority();
    REQUIRE(priority == ThreadPriority::NORMAL);
}
