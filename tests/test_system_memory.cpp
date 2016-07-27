//
// Created by Ivan Shynkarenka on 27.07.2016.
//

#include "catch.hpp"

#include "system/memory.h"

using namespace CppCommon;

TEST_CASE("Memory management", "[CppCommon][System]")
{
    REQUIRE(Memory::RamTotal() > 0);
    REQUIRE(Memory::RamFree() > 0);
}
