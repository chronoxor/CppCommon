//
// Created by Ivan Shynkarenka on 30.11.2016
//

#include "benchmark/cppbenchmark.h"

#include "system/pipe.h"

#include <functional>
#include <thread>
#include <vector>

using namespace CppCommon;

const uint64_t bytes_to_produce = 2097152;
const int item_size_from = 1;
const int item_size_to = 262144;
const auto settings = CppBenchmark::Settings().ParamRange(item_size_from, item_size_to, [](int from, int to, int& result) { int r = result; result *= 4; return r; });

void produce_consume(CppBenchmark::Context& context)
{
    const uint64_t item_size = context.x();
    const uint64_t items_to_produce = bytes_to_produce / item_size;
    uint64_t crc = 0;

    // Create communication pipe
    Pipe pipe;

    // Start consumer thread
    auto consumer = std::thread([&pipe, item_size, items_to_produce, &crc]()
    {
        uint8_t item[item_size_to];

        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Read the item from the pipe
            if (pipe.Read(item, item_size) != item_size)
                break;

            // Emulate consuming
            for (uint64_t j = 0; j < item_size; ++j)
                crc += item[j];
        }
    });

    // Start producer thread
    auto producer = std::thread([&pipe, item_size, items_to_produce]()
    {
        uint8_t item[item_size_to];

        for (uint64_t i = 0; i < items_to_produce; ++i)
        {
            // Emulate producing
            for (uint64_t j = 0; j < item_size; ++j)
                item[j] = (uint8_t)j;

            // Write the item into the pipe
            if (pipe.Write(item, item_size) != item_size)
                break;
        }
    });

    // Wait for the producer thread
    producer.join();

    // Wait for the consumer thread
    consumer.join();

    // Update benchmark metrics
    context.metrics().AddOperations(items_to_produce - 1);
    context.metrics().AddItems(items_to_produce);
    context.metrics().AddBytes(bytes_to_produce);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("Pipe", settings)
{
    produce_consume(context);
}

BENCHMARK_MAIN()
