//
// Created by Ivan Shynkarenka on 01.12.2016
//

#include "test.h"

#include "system/process.h"

using namespace CppCommon;

TEST_CASE("Process", "[CppCommon][System]")
{
    REQUIRE(Process::CurrentProcessId() > 0);
    REQUIRE(Process::ParentProcessId() > 0);
    REQUIRE(Process::CurrentProcessId() != Process::ParentProcessId());

    REQUIRE(Process::CurrentProcess().IsRunning());
    REQUIRE(Process::ParentProcess().IsRunning());
}
