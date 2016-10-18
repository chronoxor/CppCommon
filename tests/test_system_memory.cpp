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

TEST_CASE("Memory align", "[CppCommon][System]")
{
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4af, 64, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4be, 32, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4bd, 16, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4bc,  8, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4bb,  4, true) == (void*)0x7fff5ebcf4bc);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4b6,  2, true) == (void*)0x7fff5ebcf4b6);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4af,  1, true) == (void*)0x7fff5ebcf4af);

    REQUIRE(Memory::Align((void*)0x7fff5ebcf4af, 64, false) == (void*)0x7fff5ebcf480);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf47e, 32, false) == (void*)0x7fff5ebcf460);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf45d, 16, false) == (void*)0x7fff5ebcf450);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf44c,  8, false) == (void*)0x7fff5ebcf448);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf443,  4, false) == (void*)0x7fff5ebcf440);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf43a,  2, false) == (void*)0x7fff5ebcf43a);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf433,  1, false) == (void*)0x7fff5ebcf433);
}
