//
// Created by Ivan Shynkarenka on 15.02.2016.
//

#include "cppbenchmark.h"

#include "debug/stack_trace.h"

const uint64_t iterations = 1000000;

BENCHMARK("Stack trace")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += CppCommon::StackTrace().frames().size();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
