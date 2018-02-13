//
// Created by Ivan Shynkarenka on 19.01.2016
//

#include "test.h"

#include "threads/mpmc_ring_queue.h"

using namespace CppCommon;

TEST_CASE("Multiple producers / multiple consumers wait-free ring queue", "[CppCommon][Threads]")
{
    MPMCRingQueue<int> queue(4);

    REQUIRE(queue.capacity() == 4);
    REQUIRE(queue.size() == 0);

    int v = -1;

    REQUIRE(!queue.Dequeue(v));

    REQUIRE((queue.Enqueue(0) && (queue.size() == 1)));
    REQUIRE((queue.Enqueue(1) && (queue.size() == 2)));
    REQUIRE((queue.Enqueue(2) && (queue.size() == 3)));
    REQUIRE((queue.Enqueue(3) && (queue.size() == 4)));
    REQUIRE(!queue.Enqueue(4));

    REQUIRE(((queue.Dequeue(v) && (v == 0)) && (queue.size() == 3)));
    REQUIRE(((queue.Dequeue(v) && (v == 1)) && (queue.size() == 2)));
    REQUIRE(((queue.Dequeue(v) && (v == 2)) && (queue.size() == 1)));

    REQUIRE((queue.Enqueue(4) && (queue.size() == 2)));
    REQUIRE((queue.Enqueue(5) && (queue.size() == 3)));
    REQUIRE((queue.Enqueue(6) && (queue.size() == 4)));
    REQUIRE(!queue.Enqueue(7));

    REQUIRE(((queue.Dequeue(v) && (v == 3)) && (queue.size() == 3)));
    REQUIRE(((queue.Dequeue(v) && (v == 4)) && (queue.size() == 2)));
    REQUIRE(((queue.Dequeue(v) && (v == 5)) && (queue.size() == 1)));
    REQUIRE(((queue.Dequeue(v) && (v == 6)) && (queue.size() == 0)));
    REQUIRE(!queue.Dequeue(v));

    REQUIRE((queue.Enqueue(7) && (queue.size() == 1)));

    REQUIRE(((queue.Dequeue(v) && (v == 7)) && (queue.size() == 0)));
    REQUIRE(!queue.Dequeue(v));

    REQUIRE(queue.capacity() == 4);
    REQUIRE(queue.size() == 0);
}
