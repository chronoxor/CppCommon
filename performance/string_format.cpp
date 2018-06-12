//
// Created by Ivan Shynkarenka on 19.09.2016
//

#include "benchmark/cppbenchmark.h"

#include "string/format.h"

using namespace CppCommon;

BENCHMARK("format(int)", 10000000)
{
    context.metrics().AddBytes(format("test {} test", context.metrics().total_operations()).size());
}

BENCHMARK("format(double)", 10000000)
{
    context.metrics().AddBytes(format("test {} test", context.metrics().total_operations() / 1000.0).size());
}

BENCHMARK("format(string)", 10000000)
{
    context.metrics().AddBytes(format("test {} test", context.name()).size());
}

BENCHMARK("format(int, double, string)", 10000000)
{
    context.metrics().AddBytes(format("test {}.{}.{} test", context.metrics().total_operations(), context.metrics().total_operations() / 1000.0, context.name()).size());
}

BENCHMARK_MAIN()
