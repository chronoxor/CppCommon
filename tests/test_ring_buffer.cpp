//
// Created by Ivan Shynkarenka on 16.01.2016.
//

#include "catch.hpp"

#include "threads/ring_buffer.h"

using namespace CppCommon;

TEST_CASE("Wait-free ring buffer", "[CppCommon][Threads]")
{
    RingBuffer<4> buffer;

    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.size() == 0);

    char data[4];

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

    int64_t size;

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
