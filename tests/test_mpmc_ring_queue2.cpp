//
// Created by Ivan Shynkarenka on 20.01.2016.
//

#include "catch.hpp"

#include "threads/mpmc_ring_queue2.h"

using namespace CppCommon;

TEST_CASE("Multiple producers / multiple consumers wait-free ring queue 2", "[CppCommon][Threads]")
{
    MPMCRingQueue2<int> queue(4);

    REQUIRE(queue.capacity() == 3);
    REQUIRE(queue.size() == 0);

    int v = -1;

    REQUIRE(!queue.Dequeue(v));

    REQUIRE((queue.Enqueue(0) && (queue.size() == 1)));
    REQUIRE((queue.Enqueue(1) && (queue.size() == 2)));
    REQUIRE((queue.Enqueue(2) && (queue.size() == 3)));
    REQUIRE(!queue.Enqueue(3));

    REQUIRE(((queue.Dequeue(v) && (v == 0)) && (queue.size() == 2)));
    REQUIRE(((queue.Dequeue(v) && (v == 1)) && (queue.size() == 1)));

    REQUIRE((queue.Enqueue(3) && (queue.size() == 2)));
    REQUIRE((queue.Enqueue(4) && (queue.size() == 3)));
    REQUIRE(!queue.Enqueue(5));

    REQUIRE(((queue.Dequeue(v) && (v == 2)) && (queue.size() == 2)));
    REQUIRE(((queue.Dequeue(v) && (v == 3)) && (queue.size() == 1)));
    REQUIRE(((queue.Dequeue(v) && (v == 4)) && (queue.size() == 0)));
    REQUIRE(!queue.Dequeue(v));

    REQUIRE((queue.Enqueue(5) && (queue.size() == 1)));

    REQUIRE(((queue.Dequeue(v) && (v == 5)) && (queue.size() == 0)));
    REQUIRE(!queue.Dequeue(v));

    REQUIRE(queue.capacity() == 3);
    REQUIRE(queue.size() == 0);
}
