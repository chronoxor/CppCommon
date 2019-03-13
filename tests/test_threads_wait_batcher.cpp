//
// Created by Ivan Shynkarenka on 13.03.2019
//

#include "test.h"

#include "threads/wait_batcher.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Multiple producers / multiple consumers wait batcher", "[CppCommon][Threads]")
{
    WaitBatcher<int> batcher;

    REQUIRE(!batcher.closed());
    REQUIRE(batcher.size() == 0);

    std::vector<int> v;

    REQUIRE((batcher.Enqueue(0) && (batcher.size() == 1)));
    REQUIRE((batcher.Enqueue(1) && (batcher.size() == 2)));
    REQUIRE((batcher.Enqueue(2) && (batcher.size() == 3)));

    REQUIRE(((batcher.Dequeue(v) && (v.size() == 3)) && (batcher.size() == 0)));

    REQUIRE((batcher.Enqueue(3) && (batcher.size() == 1)));
    REQUIRE((batcher.Enqueue(4) && (batcher.size() == 2)));

    REQUIRE(((batcher.Dequeue(v) && (v.size() == 2)) && (batcher.size() == 0)));

    REQUIRE((batcher.Enqueue(5) && (batcher.size() == 1)));

    REQUIRE(((batcher.Dequeue(v) && (v.size() == 1)) && (batcher.size() == 0)));

    batcher.Close();

    REQUIRE(batcher.closed());
    REQUIRE(batcher.size() == 0);
}

TEST_CASE("Multiple producers / multiple consumers wait batcher threads", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 4;
    int crc = 0;

    WaitBatcher<int> batcher;

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start consumer thread
    auto consumer = std::thread([&batcher, &crc]()
    {
        std::vector<int> items;

        // Dequeue items or end consume
        while (batcher.Dequeue(items))
        {
            // Consume items
            for (auto item : items)
                crc += item;
        }
    });

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&batcher, producer, items_to_produce, producers_count]()
        {
            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
                if (!batcher.Enqueue((producer * items) + i))
                    break;
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Close the wait batcher
    batcher.Close();

    // Wait for the consumer thread
    consumer.join();

    // Check result
    REQUIRE(crc == result);
}
