//
// Created by Ivan Shynkarenka on 26.01.2016.
//

#include "benchmark/cppbenchmark.h"

#include "time/timestamp.h"

using namespace CppCommon;

const uint64_t iterations = 100000000;

BENCHMARK("UtcTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += UtcTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("LocalTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += LocalTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("NanoTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += NanoTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("RdtsTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += RdtsTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
