//
// Created by Ivan Shynkarenka on 19.04.2016
//

#include "test.h"

#include "system/shared_memory.h"

#include <cstring>

using namespace CppCommon;

TEST_CASE("Shared memory manager", "[CppCommon][System]")
{
    const char* name = "shared_memory_test";
    const char* message = "shared message";
    size_t size = 14;

    // Create new shared memory manager with the given name and size
    SharedMemory shared1(name, size);
    REQUIRE(shared1.owner());
    REQUIRE(shared1.ptr() != nullptr);

    // Write into the shared memory buffer
    std::memcpy(shared1.ptr(), message, size);

    SharedMemory shared2(name, size);
    REQUIRE(!shared2.owner());
    REQUIRE(shared2.ptr() != nullptr);

    // Read from the shared memory buffer
    REQUIRE(std::memcmp(shared1.ptr(), shared2.ptr(), size) == 0);
}
