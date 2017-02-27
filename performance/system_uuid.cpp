//
// Created by Ivan Shynkarenka on 19.08.2016
//

#include "benchmark/cppbenchmark.h"

#include "system/uuid.h"

using namespace CppCommon;

BENCHMARK("UUID::Generate()", 10000000)
{
    UUID::Generate();
}

BENCHMARK_MAIN()
