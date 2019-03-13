//
// Created by Ivan Shynkarenka on 16.01.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/spsc_ring_buffer.h"

#include <functional>
#include <thread>

using namespace CppCommon;

const uint64_t bytes_to_produce = 100000000;
const int item_size_from = 4;
const int item_size_to = 4096;
const auto settings = CppBenchmark::Settings().ParamRange(item_size_from, item_size_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

template<uint64_t N>
void produce_consume(CppBenchmark::Context& context, const std::function<void()>& wait_strategy)
{
    const int item_size = context.x();
    const uint64_t items_to_produce = bytes_to_produce / item_size;
    uint64_t crc = 0;

    // Create single producer / single consumer wait-free ring buffer
    SPSCRingBuffer buffer(N);

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

    // Start producer thread
    auto producer = std::thread([&buffer, &wait_strategy, item_size, items_to_produce]()
    {
        uint8_t item[item_size_to];

        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Emulate producing
            for (int j = 0; j < item_size; ++j)
                item[j] = (uint8_t)j;

            // Enqueue using the given waiting strategy
            while (!buffer.Enqueue(item, item_size))
                wait_strategy();
        }
    });

    // Wait for the producer thread
    producer.join();

    // Wait for the consumer thread
    consumer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().AddItems(items_to_produce);
    context.metrics().AddBytes(items_to_produce * item_size);
    context.metrics().SetCustom("SPSCRingBuffer.capacity", N);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("SPSCRingBuffer<SpinWait>", settings)
{
    produce_consume<1048576>(context, []{});
}

BENCHMARK("SPSCRingBuffer<YieldWait>", settings)
{
    produce_consume<1048576>(context, []{ std::this_thread::yield(); });
}

BENCHMARK_MAIN()
