//
// Created by Ivan Shynkarenka on 02.09.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/file_lock.h"

#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 100000;
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

    // Create file-lock synchronization primitive
    FileLock lock_master(".lock");

    // Start readers threads
    std::vector<std::thread> readers;
    for (int reader = 0; reader < readers_count; ++reader)
    {
        readers.emplace_back([&readers_crc, reader, readers_count]()
        {
            FileLock lock_slave(".lock");

            uint64_t items = (items_to_produce / readers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                ReadLocker<FileLock> locker(lock_slave);
                readers_crc += (reader * items) + i;
            }
        });
    }

    // Start writers threads
    std::vector<std::thread> writers;
    for (int writer = 0; writer < writers_count; ++writer)
    {
        writers.emplace_back([&writers_crc, writer, writers_count]()
        {
            FileLock lock_slave(".lock");

            uint64_t items = (items_to_produce / writers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                WriteLocker<FileLock> locker(lock_slave);
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

BENCHMARK("FileLock", settings)
{
    produce(context);
}

BENCHMARK_MAIN()
