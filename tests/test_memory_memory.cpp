//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "test.h"

#include "memory/memory.h"

using namespace CppCommon;

TEST_CASE("Memory management", "[CppCommon][Memory]")
{
    REQUIRE(Memory::RamTotal() > 0);
    REQUIRE(Memory::RamFree() > 0);
}

TEST_CASE("Memory align", "[CppCommon][Memory]")
{
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4af, 1));
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4b6, 2));
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4bc, 4));
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4c0, 8));
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4c0, 16));
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4c0, 32));
    REQUIRE(Memory::IsAligned((void*)0x7fff5ebcf4c0, 64));
    REQUIRE(!Memory::IsAligned((void*)0x7fff5ebcf4af, 2));
    REQUIRE(!Memory::IsAligned((void*)0x7fff5ebcf4af, 4));
    REQUIRE(!Memory::IsAligned((void*)0x7fff5ebcf4af, 8));
    REQUIRE(!Memory::IsAligned((void*)0x7fff5ebcf4af, 16));
    REQUIRE(!Memory::IsAligned((void*)0x7fff5ebcf4af, 32));
    REQUIRE(!Memory::IsAligned((void*)0x7fff5ebcf4af, 64));

    REQUIRE(Memory::Align((void*)0x7fff5ebcf4af,  1, true) == (void*)0x7fff5ebcf4af);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4b6,  2, true) == (void*)0x7fff5ebcf4b6);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4bb,  4, true) == (void*)0x7fff5ebcf4bc);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4bc,  8, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4bd, 16, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4be, 32, true) == (void*)0x7fff5ebcf4c0);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4af, 64, true) == (void*)0x7fff5ebcf4c0);

    REQUIRE(Memory::Align((void*)0x7fff5ebcf433,  1, false) == (void*)0x7fff5ebcf433);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf43a,  2, false) == (void*)0x7fff5ebcf43a);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf44c,  8, false) == (void*)0x7fff5ebcf448);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf44c,  8, false) == (void*)0x7fff5ebcf448);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf45d, 16, false) == (void*)0x7fff5ebcf450);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf47e, 32, false) == (void*)0x7fff5ebcf460);
    REQUIRE(Memory::Align((void*)0x7fff5ebcf4af, 64, false) == (void*)0x7fff5ebcf480);
}
