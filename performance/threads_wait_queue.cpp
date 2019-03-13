//
// Created by Ivan Shynkarenka on 05.10.2016
//

#include "benchmark/cppbenchmark.h"

#include "threads/wait_queue.h"

#include <functional>
#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 10000000;
const int producers_from = 1;
const int producers_to = 8;
const auto settings = CppBenchmark::Settings().ParamRange(producers_from, producers_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

template<typename T>
void produce_consume(CppBenchmark::Context& context)
{
    const int producers_count = context.x();
    uint64_t crc = 0;

    // Create multiple producers / multiple consumers wait queue
    WaitQueue<T> queue;

    // Start consumer thread
    auto consumer = std::thread([&queue, &crc]()
    {
        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Dequeue the item or end consume
            T item;
            if (!queue.Dequeue(item))
                break;

            // Consume the item
            crc += item;
        }
    });

    // Start producer threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&queue, producer, producers_count]()
        {
            uint64_t items = (items_to_produce / producers_count);
            for (uint64_t i = 0; i < items; ++i)
            {
                // Enqueue the item or end produce
                if (!queue.Enqueue((T)(items * producer + i)))
                    break;
            }
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Close the wait queue
    queue.Close();

    // Wait for the consumer thread
    consumer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().AddItems(items_to_produce);
    context.metrics().AddBytes(items_to_produce * sizeof(T));
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("WaitQueue-producers", settings)
{
    produce_consume<int>(context);
}

BENCHMARK_MAIN()
