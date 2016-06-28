//
// Created by Ivan Shynkarenka on 22.01.2016.
//

#include "benchmark/cppbenchmark.h"

#include "system/rdts.h"

const uint64_t iterations = 100000000;

BENCHMARK("RDTS")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += CppCommon::rdts();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
