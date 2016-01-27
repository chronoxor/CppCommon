//
// Created by Ivan Shynkarenka on 22.01.2016.
//

#include "catch.hpp"

#include "system/rdts.h"

using namespace CppCommon;

TEST_CASE("Read Time Stamp Counter", "[CppCommon][System]")
{
    REQUIRE(rdts() > 0);

    uint64_t prev = 0;
    for (int i = 0; i < 1000; ++i)
    {
        uint64_t next = rdts();
        REQUIRE(prev <= next);
        prev = next;
    }
}
