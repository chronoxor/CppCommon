//
// Created by Ivan Shynkarenka on 22.01.2016.
//

#include "catch.hpp"

#include "threads/mpsc_ring_queue.h"

using namespace CppCommon;

TEST_CASE("Multiple producers / single consumer wait-free ring queue", "[CppCommon][Threads]")
{
    MPSCRingQueue<int> queue(4, 4);

    REQUIRE(queue.capacity() == 4);
    REQUIRE(queue.size() == 0);

    int v = -1;

    REQUIRE(!queue.Dequeue(v));

    REQUIRE(queue.Enqueue(0));
    REQUIRE(queue.Enqueue(1));
    REQUIRE(queue.Enqueue(2));

    REQUIRE(queue.Dequeue(v));
    REQUIRE(queue.Dequeue(v));

    REQUIRE(queue.Enqueue(3));
    REQUIRE(queue.Enqueue(4));

    REQUIRE(queue.Dequeue(v));
    REQUIRE(queue.Dequeue(v));
    REQUIRE(queue.Dequeue(v));

    REQUIRE(queue.Enqueue(5));

    REQUIRE(queue.Dequeue(v));

    REQUIRE(queue.capacity() == 4);
    REQUIRE(queue.size() == 0);
}

TEST_CASE("Multiple producers / single consumer wait-free ring queue (batch mode)", "[CppCommon][Threads]")
{
    MPSCRingQueue<int> batcher(4, 4);

    REQUIRE(!batcher.Dequeue([](const int&){}));

    REQUIRE(batcher.Enqueue(0));
    REQUIRE(batcher.Enqueue(1));
    REQUIRE(batcher.Enqueue(2));

    REQUIRE(batcher.Dequeue([](const int&){}));
    REQUIRE(!batcher.Dequeue([](const int&){}));

    REQUIRE(batcher.Enqueue(3));
    REQUIRE(batcher.Enqueue(4));

    REQUIRE(batcher.Dequeue([](const int&){}));
    REQUIRE(!batcher.Dequeue([](const int&){}));

    REQUIRE(batcher.Enqueue(5));

    REQUIRE(batcher.Dequeue([](const int&){}));
    REQUIRE(!batcher.Dequeue([](const int&){}));
}
