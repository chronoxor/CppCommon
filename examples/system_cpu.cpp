/*!
    \file system_cpu.cpp
    \brief CPU management example
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "system/cpu.h"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "CPU architecture: " << CppCommon::CPU::Architecture() << std::endl;
    std::cout << "CPU affinity: " << CppCommon::CPU::Affinity() << std::endl;
    std::cout << "CPU logical cores: " << CppCommon::CPU::LogicalCores() << std::endl;
    std::cout << "CPU physical cores: " << CppCommon::CPU::PhysicalCores() << std::endl;
    std::cout << "CPU clock speed: " << CppCommon::CPU::ClockSpeed() << " Hz" << std::endl;
    std::cout << "CPU Hyper-Threading: " << (CppCommon::CPU::HyperThreading() ? "enabled" : "disabled") << std::endl;
    return 0;
}
