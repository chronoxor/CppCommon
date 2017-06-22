//
// Created by Ivan Shynkarenka on 20.06.2017
//

#include "catch.hpp"

#include "intrusive/queue.h"

using namespace CppCommon;

struct MyQueueNode : public CppCommon::Queue<MyQueueNode>::Node
{
    int value;
    MyQueueNode(int v) : value(v) {}
};

TEST_CASE("Intrusive queue", "[CppCommon][Intrusive]")
{
    CppCommon::Queue<MyQueueNode> queue;
    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);

    MyQueueNode item1(1);
    queue.Push(item1);
    REQUIRE(queue.size() == 1);
    MyQueueNode item2(2);
    queue.Push(item2);
    REQUIRE(queue.size() == 2);
    MyQueueNode item3(3);
    queue.Push(item3);
    REQUIRE(queue.size() == 3);

    REQUIRE(!queue.empty());

    int sum = 0;
    for (auto it : queue)
        sum += it.value;
    REQUIRE(sum == 6);

    MyQueueNode* node = queue.Pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(queue.size() == 2);

    node = queue.Pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(queue.size() == 1);

    queue.Push(item2);
    REQUIRE(queue.size() == 2);
    queue.Push(item1);
    REQUIRE(queue.size() == 3);

    queue.Reverse();

    node = queue.Pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(queue.size() == 2);

    node = queue.Pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(queue.size() == 1);

    node = queue.Pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(queue.size() == 0);

    REQUIRE(queue.empty());
}
