//
// Created by Ivan Shynkarenka on 05.10.2016
//

#include "test.h"

#include "threads/wait_queue.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Multiple producers / multiple consumers wait queue", "[CppCommon][Threads]")
{
    WaitQueue<int> queue;

    REQUIRE(!queue.closed());
    REQUIRE(queue.size() == 0);

    int v = -1;

    REQUIRE((queue.Enqueue(0) && (queue.size() == 1)));
    REQUIRE((queue.Enqueue(1) && (queue.size() == 2)));
    REQUIRE((queue.Enqueue(2) && (queue.size() == 3)));

    REQUIRE(((queue.Dequeue(v) && (v == 0)) && (queue.size() == 2)));
    REQUIRE(((queue.Dequeue(v) && (v == 1)) && (queue.size() == 1)));

    REQUIRE((queue.Enqueue(3) && (queue.size() == 2)));
    REQUIRE((queue.Enqueue(4) && (queue.size() == 3)));

    REQUIRE(((queue.Dequeue(v) && (v == 2)) && (queue.size() == 2)));
    REQUIRE(((queue.Dequeue(v) && (v == 3)) && (queue.size() == 1)));
    REQUIRE(((queue.Dequeue(v) && (v == 4)) && (queue.size() == 0)));

    REQUIRE((queue.Enqueue(5) && (queue.size() == 1)));

    REQUIRE(((queue.Dequeue(v) && (v == 5)) && (queue.size() == 0)));

    queue.Close();

    REQUIRE(queue.closed());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("Multiple producers / multiple consumers wait queue threads", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 4;
    int crc = 0;

    WaitQueue<int> queue;

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start consumer thread
    auto consumer = std::thread([&queue, &crc, items_to_produce]()
    {
        // Consume items
        for (int i = 0; i < items_to_produce; ++i)
        {
            int item;
            if (!queue.Dequeue(item))
                break;
            crc += item;
        }
    });

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&queue, producer, items_to_produce, producers_count]()
        {
            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
                if (!queue.Enqueue((producer * items) + i))
                    break;
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Close the wait queue
    queue.Close();

    // Wait for the consumer thread
    consumer.join();

    // Check result
    REQUIRE(crc == result);
}
