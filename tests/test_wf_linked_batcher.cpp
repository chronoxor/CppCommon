//
// Created by Ivan Shynkarenka on 19.01.2016.
//

#include "catch.hpp"

#include "threads/wf_linked_batcher.h"

using namespace CppCommon;

TEST_CASE("Wait-free linked batcher", "[CppCommon][Threads]")
{
    WFLinkedBatcher<int> batcher;

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
