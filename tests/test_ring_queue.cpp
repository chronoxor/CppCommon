//
// Created by Ivan Shynkarenka on 15.01.2016.
//

#include "catch.hpp"

#include "threads/ring_queue.h"

using namespace CppCommon;

TEST_CASE("Wait-free ring queue", "[CppCommon][Threads]")
{
    RingQueue<int> queue(4);

    REQUIRE(queue.push(0));
    REQUIRE(queue.push(1));
    REQUIRE(queue.push(2));
    REQUIRE(queue.push(3));
    REQUIRE(!queue.push(4));

    int v = -1;

    REQUIRE(queue.pop(v) && (v == 0));
    REQUIRE(queue.pop(v) && (v == 1));

    REQUIRE(queue.push(4));
    REQUIRE(queue.push(5));
    REQUIRE(!queue.push(6));

    REQUIRE(queue.pop(v) && (v == 2));
    REQUIRE(queue.pop(v) && (v == 3));
    REQUIRE(queue.pop(v) && (v == 4));
    REQUIRE(queue.pop(v) && (v == 5));
    REQUIRE(!queue.pop(v));

    REQUIRE(queue.push(6));
    REQUIRE(queue.push(7));

    REQUIRE(queue.pop(v) && (v == 6));
    REQUIRE(queue.pop(v) && (v == 7));
    REQUIRE(!queue.pop(v));
}
