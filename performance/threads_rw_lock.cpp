//
// Created by Ivan Shynkarenka on 25.05.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/rw_lock.h"

#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 10000000;
const int readers_from = 1;
const int readers_to = 32;
const int writers_from = 1;
const int writers_to = 32;
const auto settings = CppBenchmark::Settings().PairRange(readers_from, readers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; },
                                                         writers_from, writers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

void produce(CppBenchmark::Context& context)
{
    const int readers_count = context.x();
    const int writers_count = context.y();
    uint64_t readers_crc = 0;
    uint64_t writers_crc = 0;

    // Create read/write lock synchronization primitive
    RWLock lock;

    // Start readers threads
    std::vector<std::thread> readers;
    for (int reader = 0; reader < readers_count; ++reader)
    {
        readers.emplace_back([&lock, &readers_crc, reader, readers_count]()
        {
            uint64_t items = (items_to_produce / readers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                ReadLocker<RWLock> locker(lock);
                readers_crc += (reader * items) + i;
            }
        });
    }

    // Start writers threads
    std::vector<std::thread> writers;
    for (int writer = 0; writer < writers_count; ++writer)
    {
        writers.emplace_back([&lock, &writers_crc, writer, writers_count]()
        {
            uint64_t items = (items_to_produce / writers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                WriteLocker<RWLock> locker(lock);
                writers_crc += (writer * items) + i;
            }
        });
    }

    // Wait for all readers threads
    for (auto& reader : readers)
        reader.join();

    // Wait for all writers threads
    for (auto& writer : writers)
        writer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().SetCustom("CRC-Readers", readers_crc);
    context.metrics().SetCustom("CRC-Writers", writers_crc);
}

BENCHMARK("RWLock", settings)
{
    produce(context);
}

BENCHMARK_MAIN()
