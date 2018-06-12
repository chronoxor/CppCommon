//
// Created by Ivan Shynkarenka on 17.08.2017
//

#include "benchmark/cppbenchmark.h"

#include "threads/seq_lock.h"
#include "threads/thread.h"

#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 100000000;
const int readers_from = 1;
const int readers_to = 32;
const auto settings = CppBenchmark::Settings().ParamRange(readers_from, readers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

struct Data
{
    uint64_t a;
    uint64_t b;
    uint64_t c;
};

void produce(CppBenchmark::Context& context)
{
    const int readers_count = context.x();
    uint64_t writer_crc = 0;

    // Create sequential lock synchronization primitive
    SeqLock<Data> lock;

    // Start readers threads
    std::vector<std::thread> readers;
    for (int reader = 0; reader < readers_count; ++reader)
    {
        readers.emplace_back([&lock]()
        {
            for (;;)
            {
                Data data = lock.Read();
                if ((data.a + data.b + data.c) == (items_to_produce + 300))
                    return;
                Thread::Yield();
            }
        });
    }

    // Start writer threads
    std::thread writer = std::thread([&lock, &writer_crc]()
    {
        for (uint64_t i = 0; i <= items_to_produce; ++i)
        {
            lock.Write(Data{ i, i + 100, i + 200 });
            writer_crc += i;
        }
    });

    // Wait for all readers threads
    for (auto& reader : readers)
        reader.join();

    // Wait for the writer thread
    writer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().SetCustom("CRC-Writer", writer_crc);
}

BENCHMARK("SeqLock", settings)
{
    produce(context);
}

BENCHMARK_MAIN()
