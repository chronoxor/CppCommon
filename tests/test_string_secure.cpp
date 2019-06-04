//
// Created by Ivan Shynkarenka on 04.06.2019
//

#include "test.h"

#include "string/secure.h"

#include <cstring>

using namespace CppCommon;

TEST_CASE("Secure string", "[CppCommon][String]")
{
    char* pwd = nullptr;
    {
        SecureString password("password");
        pwd = password.data();
        REQUIRE(std::strcmp(pwd, "password") == 0);
    }
    REQUIRE(std::strcmp(pwd, "password") != 0);
}
