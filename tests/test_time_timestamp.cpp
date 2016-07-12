//
// Created by Ivan Shynkarenka on 26.01.2016.
//

#include "catch.hpp"

#include "time/timestamp.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Timestamp", "[CppCommon][Time]")
{
    REQUIRE(Timestamp::current() > 0);

    uint64_t prev = 0;
    for (int i = 0; i < 1000; ++i)
    {
        uint64_t next = Timestamp::current();
        REQUIRE(prev <= next);
        prev = next;
    }

    // Compatibility with std::chrono
    Timestamp timestamp = Timestamp::chrono(std::chrono::system_clock::now() + std::chrono::milliseconds(10));
    std::this_thread::sleep_until(timestamp.chrono());
}

TEST_CASE("RDTS", "[CppCommon][Time]")
{
    REQUIRE(Timestamp::rdts() > 0);

    uint64_t prev = 0;
    for (int i = 0; i < 1000; ++i)
    {
        uint64_t next = Timestamp::rdts();
        REQUIRE(prev <= next);
        prev = next;
    }
}
