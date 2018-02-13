//
// Created by Ivan Shynkarenka on 19.01.2016
//

#include "test.h"

#include "threads/mpsc_linked_batcher.h"

using namespace CppCommon;

TEST_CASE("Multiple producers / single consumer wait-free linked batcher", "[CppCommon][Threads]")
{
    MPSCLinkedBatcher<int> batcher;

    REQUIRE(!batcher.Dequeue());

    REQUIRE(batcher.Enqueue(0));
    REQUIRE(batcher.Enqueue(1));
    REQUIRE(batcher.Enqueue(2));

    REQUIRE(batcher.Dequeue());
    REQUIRE(!batcher.Dequeue());

    REQUIRE(batcher.Enqueue(3));
    REQUIRE(batcher.Enqueue(4));

    REQUIRE(batcher.Dequeue());
    REQUIRE(!batcher.Dequeue());

    REQUIRE(batcher.Enqueue(5));

    REQUIRE(batcher.Dequeue());
    REQUIRE(!batcher.Dequeue());
}
