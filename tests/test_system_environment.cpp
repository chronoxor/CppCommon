//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "test.h"

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
    REQUIRE(Environment::UnixEndLine().length() > 0);
    REQUIRE(Environment::WindowsEndLine().length() > 0);
}

TEST_CASE("Environment variables", "[CppCommon][System]")
{
    auto envars = Environment::envars();
    REQUIRE(envars.size() > 0);
    for (const auto& envar : envars)
        REQUIRE(!envar.first.empty());

    REQUIRE(Environment::GetEnvar("TestEnvar") == "");
    Environment::SetEnvar("TestEnvar", "123");
    REQUIRE(Environment::GetEnvar("TestEnvar") == "123");
    Environment::ClearEnvar("TestEnvar");
    REQUIRE(Environment::GetEnvar("TestEnvar") == "");
}
