//
// Created by Ivan Shynkarenka on 30.11.2016.
//

#include "benchmark/cppbenchmark.h"

#include "system/pipe.h"

#include <functional>
#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t items_to_produce = 1000000;

template<typename T>
void produce_consume(CppBenchmark::Context& context)
{
    uint64_t crc = 0;

    // Create communication pipe
    Pipe pipe;

    // Start consumer thread
    auto consumer = std::thread([&pipe, &crc]()
    {
        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Read the item from the pipe
            T item;
            if (pipe.Read(&item, sizeof(item)) != sizeof(item))
                break;

            // Consume the item
            crc += item;
        }
    });

    // Start producer thread
    auto producer = std::thread([&pipe]()
    {
        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Write the item into the pipe
            T item = (T)i;
            if (pipe.Write(&item, sizeof(item)) != sizeof(item))
                break;
        }
    });

    // Wait for the consumer thread
    consumer.join();

    // Wait for the producer thread
    producer.join();

    // Update benchmark metrics
    context.metrics().AddIterations(items_to_produce - 1);
    context.metrics().AddItems(items_to_produce);
    context.metrics().AddBytes(items_to_produce * sizeof(T));
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Pipe")
{
    produce_consume<int>(context);
}

BENCHMARK_MAIN()
