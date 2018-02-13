//
// Created by Ivan Shynkarenka on 13.07.2016
//

#include <benchmark/cppbenchmark.h>

#include "time/time.h"

using namespace CppCommon;

const uint64_t iterations = 10000000;

BENCHMARK("UtcTime()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += UtcTime().second();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("LocalTime()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += LocalTime().second();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Time::utcstamp()")
{
    uint64_t crc = 0;

    UtcTime time = UtcTime();
    for (uint64_t i = 0; i < iterations; ++i)
        crc += time.utcstamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Time::localstamp()")
{
    uint64_t crc = 0;

    UtcTime time = UtcTime();
    for (uint64_t i = 0; i < iterations; ++i)
        crc += time.localstamp().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
