//
// Created by Ivan Shynkarenka on 26.01.2016
//

#include "test.h"

#include "threads/mpsc_ring_buffer.h"

using namespace CppCommon;

TEST_CASE("Multiple producers / single consumer wait-free ring buffer", "[CppCommon][Threads]")
{
    MPSCRingBuffer buffer(4);

    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.size() == 0);

    char data[4];
    size_t size;

    REQUIRE(!buffer.Dequeue(data, size = 4));

    REQUIRE(buffer.Enqueue(data, 1));
    REQUIRE(buffer.size() == 1);

    REQUIRE((buffer.Dequeue(data, size = 4) && (size == 1)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(buffer.Enqueue(data, 2));
    REQUIRE(buffer.size() == 2);

    REQUIRE((buffer.Dequeue(data, size = 4) && (size == 2)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(buffer.Enqueue(data, 3));
    REQUIRE(buffer.size() == 3);

    REQUIRE((buffer.Dequeue(data, size = 4) && (size == 3)));
    REQUIRE(buffer.size() == 0);

    REQUIRE(!buffer.Dequeue(data, size = 4));

    REQUIRE(buffer.capacity() == 3);
    REQUIRE(buffer.size() == 0);
}
