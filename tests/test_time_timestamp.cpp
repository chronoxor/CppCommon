//
// Created by Ivan Shynkarenka on 26.01.2016.
//

#include "catch.hpp"

#include "time/timestamp.h"

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
