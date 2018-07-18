//
// Created by Ivan Shynkarenka on 18.08.2017
//

#include "benchmark/cppbenchmark.h"

#include "common/function.h"

using namespace CppCommon;

class Class
{
public:
    void test(int64_t data) { _data = data; }

private:
    volatile int64_t _data;
};

BENCHMARK("std::function: create & invoke")
{
    static Class instance;

    // Create the function
    std::function<void (int64_t)> function = std::bind(&Class::test, &instance, std::placeholders::_1);

    // Call the function
    function(context.metrics().total_operations());
}

BENCHMARK("std::function: invoke")
{
    static Class instance;
    static std::function<void (int64_t)> function = std::bind(&Class::test, &instance, std::placeholders::_1);

    // Call the function
    function(context.metrics().total_operations());
}

BENCHMARK("CppCommon::Function: create & invoke")
{
    static Class instance;

    // Create the function
    CppCommon::Function<void (int64_t)> function = std::bind(&Class::test, &instance, std::placeholders::_1);

    // Call the function
    function(context.metrics().total_operations());
}

BENCHMARK("CppCommon::Function: invoke")
{
    static Class instance;
    static CppCommon::Function<void (int64_t)> function = std::bind(&Class::test, &instance, std::placeholders::_1);

    // Call the function
    function(context.metrics().total_operations());
}

BENCHMARK_MAIN()
