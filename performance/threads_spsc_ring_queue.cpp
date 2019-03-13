//
// Created by Ivan Shynkarenka on 16.01.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/spsc_ring_queue.h"

#include <functional>
#include <thread>

using namespace CppCommon;

const uint64_t items_to_produce = 100000000;

template<typename T, uint64_t N>
void produce_consume(CppBenchmark::Context& context, const std::function<void()>& wait_strategy)
{
    uint64_t crc = 0;

    // Create single producer / single consumer wait-free ring queue
    SPSCRingQueue<T> queue(N);

    // Start consumer thread
    auto consumer = std::thread([&queue, &wait_strategy, &crc]()
    {
        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Dequeue using the given waiting strategy
            T item;
            while (!queue.Dequeue(item))
                wait_strategy();

            // Consume the item
            crc += item;
        }
    });

    // Start producer thread
    auto producer = std::thread([&queue, &wait_strategy]()
    {
        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Enqueue using the given waiting strategy
            while (!queue.Enqueue((T)i))
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
    context.metrics().AddBytes(items_to_produce * sizeof(T));
    context.metrics().SetCustom("SPSCRingQueue.capacity", N);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("SPSCRingQueue<SpinWait>")
{
    produce_consume<int, 1048576>(context, []{});
}

BENCHMARK("SPSCRingQueue<YieldWait>")
{
    produce_consume<int, 1048576>(context, []{ std::this_thread::yield(); });
}

BENCHMARK_MAIN()
