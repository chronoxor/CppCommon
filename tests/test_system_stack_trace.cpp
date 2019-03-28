//
// Created by Ivan Shynkarenka on 11.02.2016
//

#include "test.h"

#include "system/stack_trace.h"
#include "system/stack_trace_manager.h"

#include <thread>

using namespace CppCommon;

namespace {

StackTrace function1()
{
    return __STACK__;
}

StackTrace function2()
{
    return function1();
}

StackTrace function3()
{
    return function2();
}

void validate(const std::vector<StackTrace::Frame>& frames)
{
    REQUIRE(!frames.empty());
    for (const auto& frame : frames)
    {
        REQUIRE(frame.address != nullptr);
    }
}

void equal(const std::vector<StackTrace::Frame>& frames1, const std::vector<StackTrace::Frame>& frames2, int count)
{
    for (int i = 0; i < count; ++i)
    {
        REQUIRE(frames1[i].address == frames2[i].address);
        REQUIRE(frames1[i].module == frames2[i].module);
        REQUIRE(frames1[i].function == frames2[i].function);
        REQUIRE(frames1[i].filename == frames2[i].filename);
        REQUIRE(frames1[i].line == frames2[i].line);
    }
}

} // namespace

TEST_CASE("Stack trace snapshot provider", "[CppCommon][System]")
{
    StackTraceManager::Initialize();

    auto root = __STACK__;
    validate(root.frames());

    auto trace = function3();
    validate(trace.frames());

    int frames = (int)trace.frames().size() - (int)root.frames().size();
    REQUIRE(frames <= 3);

    std::thread([&trace, frames]()
    {
        auto thread = function3();
        validate(thread.frames());
        equal(thread.frames(), trace.frames(), frames);
    }).join();

    StackTraceManager::Cleanup();
}
