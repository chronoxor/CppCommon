//
// Created by Ivan Shynkarenka on 10.02.2016.
//

#include "catch.hpp"

#include "errors/system_error.h"

using namespace CppCommon;

TEST_CASE("System error wrapper", "[CppCommon][Errors]")
{
    REQUIRE(SystemError::GetLast() == 0);

    SystemError::SetLast(123);
    REQUIRE(SystemError::GetLast() == 123);
    SystemError::ClearLast();

    REQUIRE(SystemError::GetLast() == 0);

    REQUIRE(!SystemError::Convert(SystemError::GetLast()).empty());
}
