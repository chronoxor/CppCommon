//
// Created by Ivan Shynkarenka on 15.02.2016.
//

#include "benchmark/cppbenchmark.h"

#include "system/stack_trace.h"

using namespace CppCommon;

const uint64_t iterations = 1000000;

BENCHMARK("StackTrace")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += StackTrace().frames().size();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
