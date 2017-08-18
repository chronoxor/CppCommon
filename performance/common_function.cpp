//
// Created by Ivan Shynkarenka on 18.08.2017
//

#include "benchmark/cppbenchmark.h"

#include "common/function.h"

using namespace CppCommon;

struct Data
{
    int64_t a;
    int64_t b;
    int64_t c;
};

BENCHMARK("std::function: create & invoke", 100000000)
{
    auto iteration = context.metrics().total_iterations();

    // Create the function
    volatile int64_t data1 = 0;
    volatile Data data2 = { 0, 0, 0};
    std::function<void ()> function = [&data1, &data2, iteration]
    {
        data1 = iteration;
        data2.a = iteration;
        data2.b = iteration;
        data2.c = iteration;
    };

    // Call the function
    function();
}

BENCHMARK("std::function: invoke", 100000000)
{
    // Create the function once
    volatile int64_t data1 = 0;
    volatile Data data2 = { 0, 0, 0};
    static std::function<void (int64_t)> function = [&data1, &data2](int64_t iteration)
    {
        data1 = iteration;
        data2.a = iteration;
        data2.b = iteration;
        data2.c = iteration;
    };

    auto iteration = context.metrics().total_iterations();

    // Call the function
    function(iteration);
}

BENCHMARK("CppCommon::Function: create & invoke", 100000000)
{
    auto iteration = context.metrics().total_iterations();

    // Create the function
    volatile int64_t data1 = 0;
    volatile Data data2 = { 0, 0, 0};
    CppCommon::Function<void ()> function = [&data1, &data2, iteration]
    {
        data1 = iteration;
        data2.a = iteration;
        data2.b = iteration;
        data2.c = iteration;
    };

    // Call the function
    function();
}

BENCHMARK("CppCommon::Function: invoke", 100000000)
{
    // Create the function once
    volatile int64_t data1 = 0;
    volatile Data data2 = { 0, 0, 0};
    static CppCommon::Function<void (int64_t)> function = [&data1, &data2](int64_t iteration)
    {
        data1 = iteration;
        data2.a = iteration;
        data2.b = iteration;
        data2.c = iteration;
    };

    auto iteration = context.metrics().total_iterations();

    // Call the function
    function(iteration);
}

BENCHMARK_MAIN()
