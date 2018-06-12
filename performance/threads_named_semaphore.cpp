//
// Created by Ivan Shynkarenka on 25.05.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/named_semaphore.h"

#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 1000000;
const int semaphore_from = 1;
const int semaphore_to = 32;
const int producers_from = 1;
const int producers_to = 32;
const auto settings = CppBenchmark::Settings().PairRange(semaphore_from, semaphore_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; },
                                                         producers_from, producers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

void produce(CppBenchmark::Context& context)
{
    const int semaphore_count = context.x();
    const int producers_count = context.y();
    uint64_t crc = 0;

    // Create named semaphore master
    NamedSemaphore lock_master("named_semaphore_perf", semaphore_count);

    // Start producer threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&crc, producer, producers_count, semaphore_count]()
        {
            // Create named semaphore slave
            NamedSemaphore lock_slave("named_semaphore_perf", semaphore_count);

            uint64_t items = (items_to_produce / producers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                Locker<NamedSemaphore> locker(lock_slave);
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

BENCHMARK("NamedSemaphore", settings)
{
    produce(context);
}

BENCHMARK_MAIN()
