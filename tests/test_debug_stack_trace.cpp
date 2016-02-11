//
// Created by Ivan Shynkarenka on 11.02.2016.
//

#include "catch.hpp"

#include "debug/stack_trace.h"
#include "debug/stack_trace_manager.h"

using namespace CppCommon;

TEST_CASE("Stack trace snapshot provider", "[CppCommon][Debug]")
{
    StackTraceManager::Initialize();

    StackTraceManager::Cleanup();
}
