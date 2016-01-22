//
// Created by Ivan Shynkarenka on 16.01.2016.
//

#include "catch.hpp"

#include "threads/spsc_ring_buffer.h"

using namespace CppCommon;

TEST_CASE("Single producer / single consumer wait-free ring buffer", "[CppCommon][Threads]")
{
    SPSCRingBuffer buffer(4);

    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.size() == 0);

    char data[4];
    int64_t size;

    REQUIRE(!buffer.Dequeue(data, size = 0));
    REQUIRE(!buffer.Dequeue(data, size = 1));

    REQUIRE(!buffer.Enqueue(data, 4));

    REQUIRE(buffer.Enqueue(data, 1));
    REQUIRE(buffer.size() == 1);

    REQUIRE(!buffer.Enqueue(data, 3));

    REQUIRE(buffer.Enqueue(data, 1));
    REQUIRE(buffer.size() == 2);

    REQUIRE(!buffer.Enqueue(data, 2));

    REQUIRE(buffer.Enqueue(data, 1));
    REQUIRE(buffer.size() == 3);

    REQUIRE(!buffer.Enqueue(data, 1));
    REQUIRE(!buffer.Enqueue(data, 0));

    REQUIRE((buffer.Dequeue(data, size = 3) && (size == 3)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(!buffer.Dequeue(data, size = 3));

    REQUIRE(buffer.Enqueue(data, 2));
    REQUIRE(buffer.size() == 2);

    REQUIRE((buffer.Dequeue(data, size = 2) && (size == 2)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(!buffer.Dequeue(data, size = 2));

    REQUIRE(buffer.Enqueue(data, 3));
    REQUIRE(buffer.size() == 3);

    REQUIRE((buffer.Dequeue(data, size = 3) && (size == 3)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(!buffer.Dequeue(data, size = 1));

    REQUIRE(buffer.Enqueue(data, 3));
    REQUIRE(buffer.size() == 3);

    REQUIRE((buffer.Dequeue(data, size = 1) && (size == 1)));
    REQUIRE(buffer.size() == 2);

    REQUIRE((buffer.Dequeue(data, size = 2) && (size == 2)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(!buffer.Dequeue(data, size = 1));
    REQUIRE(!buffer.Dequeue(data, size = 0));

    REQUIRE(buffer.capacity() == 3);
}
