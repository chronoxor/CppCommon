//
// Created by Ivan Shynkarenka on 26.01.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/mpsc_ring_buffer.h"

#include <functional>
#include <thread>

using namespace CppCommon;

const uint64_t bytes_to_produce = 134217728;
const int item_size_from = 4;
const int item_size_to = 4096;
const int producers_from = 1;
const int producers_to = 8;
const auto settings = CppBenchmark::Settings().PairRange(item_size_from, item_size_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; },
                                                         producers_from, producers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

template<uint64_t N>
void produce_consume(CppBenchmark::Context& context, const std::function<void()>& wait_strategy)
{
    const int item_size = context.x();
    const int producers_count = context.y();
    const uint64_t items_to_produce = bytes_to_produce / item_size;
    uint64_t crc = 0;

    // Create multiple producers / single consumer wait-free ring buffer
    MPSCRingBuffer buffer(N);

    // Start consumer thread
    auto consumer = std::thread([&buffer, &wait_strategy, item_size, items_to_produce, &crc]()
    {
        // Use big items buffer to enable items batching
        uint8_t* items = new uint8_t[N];

        for (uint64_t i = 0; i < items_to_produce;)
        {
            // Dequeue using the given waiting strategy
            size_t size;
            while (!buffer.Dequeue(items, size = N))
                wait_strategy();

            // Emulate consuming
            for (uint64_t j = 0; j < size; ++j)
                crc += items[j];

            // Increase the items counter
            i += size / item_size;
        }

        // Delete items buffer
        delete[] items;
    });

    // Start producer threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&buffer, &wait_strategy, item_size, items_to_produce, producers_count]()
        {
            uint8_t item[item_size_to];

            uint64_t items = (items_to_produce / producers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                // Emulate producing
                for (int j = 0; j < item_size; ++j)
                    item[j] = (uint8_t)j;

                // Enqueue using the given waiting strategy
                while (!buffer.Enqueue(item, item_size))
                    wait_strategy();
            }
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Wait for the consumer thread
    consumer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().AddItems(items_to_produce);
    context.metrics().AddBytes(items_to_produce * item_size);
    context.metrics().SetCustom("MPSCRingBuffer.capacity", N);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("MPSCRingBuffer<SpinWait>", settings)
{
    produce_consume<1048576>(context, []{});
}

BENCHMARK("MPSCRingBuffer<YieldWait>", settings)
{
    produce_consume<1048576>(context, []{ std::this_thread::yield(); });
}

BENCHMARK_MAIN()
