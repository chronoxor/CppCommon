//
// Created by Ivan Shynkarenka on 15.01.2016.
//

#include "catch.hpp"

#include "threads/ring_queue.h"

using namespace CppCommon;

TEST_CASE("Wait-free ring queue", "[CppCommon][Threads]")
{
    RingQueue<int, 4> queue;

    REQUIRE(queue.Push(0));
    REQUIRE(queue.Push(1));
    REQUIRE(queue.Push(2));
    REQUIRE(!queue.Push(3));

    int v = -1;

    REQUIRE((queue.Pop(v) && (v == 0)));
    REQUIRE((queue.Pop(v) && (v == 1)));

    REQUIRE(queue.Push(3));
    REQUIRE(queue.Push(4));
    REQUIRE(!queue.Push(5));

    REQUIRE((queue.Pop(v) && (v == 2)));
    REQUIRE((queue.Pop(v) && (v == 3)));
    REQUIRE((queue.Pop(v) && (v == 4)));
    REQUIRE(!queue.Pop(v));

    REQUIRE(queue.Push(5));

    REQUIRE((queue.Pop(v) && (v == 5)));
    REQUIRE(!queue.Pop(v));
}
