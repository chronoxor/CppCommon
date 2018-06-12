//
// Created by Ivan Shynkarenka on 26.05.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/named_critical_section.h"

#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 10000000;
const int producers_from = 1;
const int producers_to = 32;
const auto settings = CppBenchmark::Settings().ParamRange(producers_from, producers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

void produce(CppBenchmark::Context& context)
{
    const int producers_count = context.x();
    uint64_t crc = 0;

    // Create named critical section master
    NamedCriticalSection lock_master("named_critical_section_perf");

    // Start producer threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&crc, producer, producers_count]()
        {
            // Create named critical section slave
            NamedCriticalSection lock_slave("named_critical_section_perf");

            uint64_t items = (items_to_produce / producers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                Locker<NamedCriticalSection> locker(lock_slave);
                crc += (producer * items) + i;
            }
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("NamedCriticalSection", settings)
{
    produce(context);
}

BENCHMARK_MAIN()
