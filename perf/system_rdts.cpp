//
// Created by Ivan Shynkarenka on 22.01.2016.
//

#include "cppbenchmark.h"

#include "system/rdts.h"

const int64_t iterations = 100000000;

BENCHMARK("RDTS")
{
    int64_t crc = 0;

    for (int64_t i = 0; i < iterations; ++i)
        crc += CppCommon::RDTS::current();

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
