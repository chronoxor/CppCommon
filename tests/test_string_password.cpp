//
// Created by Ivan Shynkarenka on 04.06.2019
//

#include "test.h"

#include "memory/memory.h"
#include "string/password.h"

#include <cstring>

using namespace CppCommon;

TEST_CASE("Password", "[CppCommon][String]")
{
    char* raw = nullptr;
    {
        std::password password("password");
        raw = password.data();
        REQUIRE(std::strcmp(raw, "password") == 0);
        REQUIRE(!Memory::IsZero(raw, 8));
    }
    REQUIRE(std::strcmp(raw, "password") != 0);
    REQUIRE(Memory::IsZero(raw, 8));
}
