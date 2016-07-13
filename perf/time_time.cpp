//
// Created by Ivan Shynkarenka on 13.07.2016.
//

#include "benchmark/cppbenchmark.h"

#include "time/time.h"

using namespace CppCommon;

const uint64_t iterations = 10000000;

BENCHMARK("Time-UTC")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += Time::utc().nanosecond();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Time-Local")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += Time::local().nanosecond();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Time-Timestamp")
{
    uint64_t crc = 0;

    Time time = Time::utc();
    for (uint64_t i = 0; i < iterations; ++i)
        crc += time.timestamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
