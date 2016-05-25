//
// Created by Ivan Shynkarenka on 25.05.2016.
//

#include "cppbenchmark.h"

#include "threads/named_rw_lock.h"

#include <functional>
#include <thread>
#include <vector>

const uint64_t items_to_read = 10000000;
const uint64_t items_to_write = 10000000;
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

    // Create named read/write lock master
    CppCommon::NamedRWLock lock("named_rw_lock_perf");

    // Start readers threads
    std::vector<std::thread> readers;
    for (int reader = 0; reader < readers_count; ++reader)
    {
        readers.push_back(std::thread([&readers_crc, reader, readers_count]()
        {
            // Create named read/write lock slave
            CppCommon::NamedRWLock lock("named_rw_lock_perf");

            uint64_t items = (items_to_read / readers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                CppCommon::ReadLocker<CppCommon::NamedRWLock> locker(lock);
                readers_crc += (reader * items) + i;
            }
        }));
    }

    // Start writers threads
    std::vector<std::thread> writers;
    for (int writer = 0; writer < writers_count; ++writer)
    {
        writers.push_back(std::thread([&writers_crc, writer, writers_count]()
        {
            // Create named read/write lock slave
            CppCommon::NamedRWLock lock("named_rw_lock_perf");

            uint64_t items = (items_to_write / writers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                CppCommon::WriteLocker<CppCommon::NamedRWLock> locker(lock);
                writers_crc += (writer * items) + i;
            }
        }));
    }

    // Wait for all readers threads
    for (auto& reader : readers)
        reader.join();

    // Wait for all writers threads
    for (auto& writer : writers)
        writer.join();

    // Update benchmark metrics
    context.metrics().AddIterations(items_to_read + items_to_write - 1);
    context.metrics().SetCustom("CRC-Readers", readers_crc);
    context.metrics().SetCustom("CRC-Writers", writers_crc);
}

BENCHMARK("Named read/write lock", settings)
{
    produce(context);
}

BENCHMARK_MAIN()
