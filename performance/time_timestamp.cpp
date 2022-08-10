//
// Created by Ivan Shynkarenka on 26.01.2016
//

#include "benchmark/cppbenchmark.h"

#include "time/timestamp.h"

using namespace CppCommon;

const uint64_t operations = 100000000;

BENCHMARK("EpochTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < operations; ++i)
        crc += EpochTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("UtcTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < operations; ++i)
        crc += UtcTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("LocalTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < operations; ++i)
        crc += LocalTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("NanoTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < operations; ++i)
        crc += NanoTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("RdtsTimestamp()")
{
    uint64_t crc = 0;

    for (uint64_t i = 0; i < operations; ++i)
        crc += RdtsTimestamp().total();

    // Update benchmark metrics
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
