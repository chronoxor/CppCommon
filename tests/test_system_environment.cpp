//
// Created by Ivan Shynkarenka on 27.07.2016.
//

#include "catch.hpp"

#include "system/environment.h"

using namespace CppCommon;

TEST_CASE("Environment management", "[CppCommon][System]")
{
    REQUIRE((Environment::Is32BitOS() || Environment::Is64BitOS()));
    REQUIRE((Environment::Is32BitProcess() || Environment::Is64BitProcess()));
    REQUIRE((Environment::IsDebug() || Environment::IsRelease()));
    REQUIRE((Environment::IsBigEndian() || Environment::IsLittleEndian()));
    REQUIRE(Environment::OSVersion().length() > 0);
    REQUIRE(Environment::EndLine().length() > 0);
}

TEST_CASE("Environment variables", "[CppCommon][System]")
{
    auto envars = CppCommon::Environment::envars();
    REQUIRE(envars.size() > 0);
    for (auto& envar : envars)
        REQUIRE(!envar.first.empty());

    REQUIRE(CppCommon::Environment::GetEnvar("TestEnvar") == "");
    CppCommon::Environment::SetEnvar("TestEnvar", "123");
    REQUIRE(CppCommon::Environment::GetEnvar("TestEnvar") == "123");
    CppCommon::Environment::ClearEnvar("TestEnvar");
    REQUIRE(CppCommon::Environment::GetEnvar("TestEnvar") == "");
}
