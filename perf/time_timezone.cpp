//
// Created by Ivan Shynkarenka on 18.07.2016.
//

#include "benchmark/cppbenchmark.h"

#include "time/timezone.h"

using namespace CppCommon;

const uint64_t iterations = 10000000;

BENCHMARK("Timezone::utc()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += Timezone::utc().total().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Timezone::local()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < iterations; ++i)
        crc += Timezone::local().total().total();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
