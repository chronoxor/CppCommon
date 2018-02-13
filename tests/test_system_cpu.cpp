//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "test.h"

#include "system/cpu.h"

using namespace CppCommon;

TEST_CASE("CPU management", "[CppCommon][System]")
{
    REQUIRE(!CPU::Architecture().empty());
    REQUIRE(CPU::Affinity() > 0);
    REQUIRE(CPU::LogicalCores() > 0);
    REQUIRE(CPU::PhysicalCores() > 0);
    REQUIRE(CPU::TotalCores().first == CPU::LogicalCores());
    REQUIRE(CPU::TotalCores().second == CPU::PhysicalCores());
    REQUIRE(CPU::ClockSpeed() > 0);
    REQUIRE((CPU::HyperThreading() || !CPU::HyperThreading()));
}
