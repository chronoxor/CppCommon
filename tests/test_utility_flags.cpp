//
// Created by Ivan Shynkarenka on 25.08.2016.
//

#include "catch.hpp"

#include "utility/flags.h"

using namespace CppCommon;

enum class MyFlags
{
    None  = 0x0,
    One   = 0x1,
    Two   = 0x2,
    Three = 0x4,
    Four  = 0x8
};

ENUM_FLAGS(MyFlags)

TEST_CASE("Enum-based flags", "[CppCommon][Utility]")
{
    Flags<MyFlags> init;
    REQUIRE(init.value() == MyFlags::None);
    REQUIRE(init.underlying() == 0);
    REQUIRE(init.bitset().to_ulong() == 0);

    Flags<MyFlags> single = MyFlags::Two;
    REQUIRE(single.value() == MyFlags::Two);
    REQUIRE(single.underlying() == 2);
    REQUIRE(single.bitset().to_ulong() == 2);

    Flags<MyFlags> combination = MyFlags::One | MyFlags::Three;
    REQUIRE((combination & MyFlags::One));
    REQUIRE(!(combination & MyFlags::Two));
    REQUIRE((combination & MyFlags::Three));
    REQUIRE(!(combination & MyFlags::Four));
    REQUIRE(combination.underlying() == 5);
    REQUIRE(combination.bitset().to_ulong() == 5);

    combination = ~combination;
    REQUIRE(!(combination & MyFlags::One));
    REQUIRE((combination & MyFlags::Two));
    REQUIRE(!(combination & MyFlags::Three));
    REQUIRE((combination & MyFlags::Four));
    REQUIRE(combination.underlying() == ~5);
    REQUIRE(combination.bitset().to_ulong() == ~5);
}
