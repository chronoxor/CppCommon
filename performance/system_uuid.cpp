//
// Created by Ivan Shynkarenka on 19.08.2016
//

#include "benchmark/cppbenchmark.h"

#include "system/uuid.h"

using namespace CppCommon;

BENCHMARK("UUID::Nil()")
{
    UUID::Nil();
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
