//
// Created by Ivan Shynkarenka on 19.08.2016
//

#include "benchmark/cppbenchmark.h"

#include "system/uuid.h"

using namespace CppCommon;

BENCHMARK("UUID::Empty()")
{
    UUID::Empty();
}

BENCHMARK("UUID::Sequential()")
{
    UUID::Sequential();
}

BENCHMARK("UUID::Random()")
{
    UUID::Random();
}

BENCHMARK_MAIN()
