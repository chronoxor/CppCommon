//
// Created by Ivan Shynkarenka on 05.10.2016
//

#include "test.h"

#include "threads/wait_ring.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Multiple producers / multiple consumers wait ring", "[CppCommon][Threads]")
{
    WaitRing<int> ring(4);

    REQUIRE(!ring.closed());
    REQUIRE(ring.capacity() == 3);
    REQUIRE(ring.size() == 0);

    int v = -1;

    REQUIRE((ring.Enqueue(0) && (ring.size() == 1)));
    REQUIRE((ring.Enqueue(1) && (ring.size() == 2)));
    REQUIRE((ring.Enqueue(2) && (ring.size() == 3)));

    REQUIRE(((ring.Dequeue(v) && (v == 0)) && (ring.size() == 2)));
    REQUIRE(((ring.Dequeue(v) && (v == 1)) && (ring.size() == 1)));

    REQUIRE((ring.Enqueue(3) && (ring.size() == 2)));
    REQUIRE((ring.Enqueue(4) && (ring.size() == 3)));

    REQUIRE(((ring.Dequeue(v) && (v == 2)) && (ring.size() == 2)));
    REQUIRE(((ring.Dequeue(v) && (v == 3)) && (ring.size() == 1)));
    REQUIRE(((ring.Dequeue(v) && (v == 4)) && (ring.size() == 0)));

    REQUIRE((ring.Enqueue(5) && (ring.size() == 1)));

    REQUIRE(((ring.Dequeue(v) && (v == 5)) && (ring.size() == 0)));

    ring.Close();

    REQUIRE(ring.closed());
    REQUIRE(ring.capacity() == 3);
    REQUIRE(ring.size() == 0);
}

TEST_CASE("Multiple producers / multiple consumers wait ring threads", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 4;
    int crc = 0;

    WaitRing<int> ring(1024);

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start consumer thread
    auto consumer = std::thread([&ring, &crc, items_to_produce]()
    {
        // Consume items
        for (int i = 0; i < items_to_produce; ++i)
        {
            int item;
            if (!ring.Dequeue(item))
                break;
            crc += item;
        }
    });

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&ring, producer, items_to_produce, producers_count]()
        {
            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
                if (!ring.Enqueue((producer * items) + i))
                    break;
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Close the wait ring
    ring.Close();

    // Wait for the consumer thread
    consumer.join();

    // Check result
    REQUIRE(crc == result);
}
