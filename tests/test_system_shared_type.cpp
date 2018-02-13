//
// Created by Ivan Shynkarenka on 23.05.2016
//

#include "test.h"

#include "system/shared_type.h"

#include <string>

using namespace CppCommon;

TEST_CASE("Shared memory type", "[CppCommon][System]")
{
    std::string name = "shared_type_test";

    // Create new shared memory type with the given name
    SharedType<int> shared1(name);
    REQUIRE(shared1);
    REQUIRE(shared1.owner());
    REQUIRE(shared1.ptr() != nullptr);

    // Assign a new value to the shared memory type
    *shared1 = 123;

    SharedType<int> shared2(name);
    REQUIRE(shared2);
    REQUIRE(!shared2.owner());
    REQUIRE(shared2.ptr() != nullptr);

    // Check the value of the shared memory type
    REQUIRE(shared1.ref() == *shared2);
}
