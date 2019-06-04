//
// Created by Ivan Shynkarenka on 04.06.2019
//

#include "test.h"

#include "string/secure.h"

using namespace CppCommon;

TEST_CASE("Secure string", "[CppCommon][String]")
{
    char* pwd = nullptr;
    {
        SecureString password("password");
        pwd = password.data();
        REQUIRE(strcmp(pwd, "password") == 0);
    }
    REQUIRE(strcmp(pwd, "password") != 0);
}
