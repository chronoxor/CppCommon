//
// Created by Ivan Shynkarenka on 23.05.2016.
//

#include "catch.hpp"

#include "system/shared_type.h"

#include <string>

using namespace CppCommon;

TEST_CASE("Shared memory type wrapper", "[CppCommon][System]")
{
    std::string name = "shared_type_test";
    std::string message = "shared message";

    // Create new shared memory type wrapper with the given name
    SharedType<std::string> shared1(name);
    REQUIRE(shared1);
    REQUIRE(shared1.owner());
    REQUIRE(shared1.ptr() != nullptr);

    // Assign a new value to the shared memory type wrapper
    *shared1 = message;

    SharedType<std::string> shared2(name);
    REQUIRE(shared2);
    REQUIRE(!shared2.owner());
    REQUIRE(shared2.ptr() != nullptr);

    // Check the value of the shared memory type wrapper
    REQUIRE(shared1.ref() == *shared2);
}
