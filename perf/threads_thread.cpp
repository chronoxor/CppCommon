//
// Created by Ivan Shynkarenka on 27.01.2016.
//

#include "benchmark/cppbenchmark.h"

#include "threads/thread.h"
#include "time/timestamp.h"

using namespace CppCommon;

const uint64_t sleep_iterations = 10;
const uint64_t yield_iterations = 1000000;

BENCHMARK("Thread::Sleep()")
{
    uint64_t iterations = sleep_iterations;
    uint64_t crc = 0;

    double maxlatency = std::numeric_limits<double>::min();
    double minlatency = std::numeric_limits<double>::max();
    int64_t previous = Timestamp::nano();
    int64_t count = 0;

    for (uint64_t i = 0; i < iterations; ++i)
    {
        Thread::Sleep(100);

        int64_t current = Timestamp::nano();
        int64_t duration = current - previous;
        double latency = (double)duration / ++count;
        if (duration > 0)
        {
            if (latency < minlatency)
            {
                minlatency = latency;
                context.metrics().SetCustom("latency-min", minlatency);
            }
            if (latency > maxlatency)
            {
                maxlatency = latency;
                context.metrics().SetCustom("latency-max", maxlatency);
            }
            previous = current;
            count = 0;
        }

        crc += i;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Thread::Yield()")
{
    uint64_t iterations = yield_iterations;
    uint64_t crc = 0;

    double maxlatency = std::numeric_limits<double>::min();
    double minlatency = std::numeric_limits<double>::max();
    int64_t previous = Timestamp::nano();
    int64_t count = 0;

    for (uint64_t i = 0; i < iterations; ++i)
    {
        Thread::Yield();

        int64_t current = Timestamp::nano();
        int64_t duration = current - previous;
        double latency = (double)duration / ++count;
        if (duration > 0)
        {
            if (latency < minlatency)
            {
                minlatency = latency;
                context.metrics().SetCustom("latency-min", minlatency);
            }
            if (latency > maxlatency)
            {
                maxlatency = latency;
                context.metrics().SetCustom("latency-max", maxlatency);
            }
            previous = current;
            count = 0;
        }

        crc += i;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(iterations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
