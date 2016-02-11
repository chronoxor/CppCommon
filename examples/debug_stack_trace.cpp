/*!
    \file debug_stack_trace.cpp
    \brief Stack trace snapshot provider example
    \author Ivan Shynkarenka
    \date 11.02.2016
    \copyright MIT License
*/

#include "debug/stack_trace.h"
#include "debug/stack_trace_manager.h"

#include <iostream>

int main(int argc, char** argv)
{
    // Initialize stack trace manager of the current process
    CppCommon::StackTraceManager::Initialize();

    std::cout << "Stack trace: " << std::endl << __STACK__ << std::endl;

    // Cleanup stack trace manager of the current process
    CppCommon::StackTraceManager::Cleanup();

    return 0;
}
