//
// Created by Ivan Shynkarenka on 20.06.2017
//

#include "test.h"

#include "containers/queue.h"

using namespace CppCommon;

namespace {

struct MyQueueNode : public Queue<MyQueueNode>::Node
{
    int value;

    explicit MyQueueNode(int v) : value(v) {}
};

} // namespace

TEST_CASE("Intrusive queue", "[CppCommon][Containers]")
{
    Queue<MyQueueNode> queue;
    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);

    MyQueueNode item1(1);
    queue.push(item1);
    REQUIRE(queue.size() == 1);
    MyQueueNode item2(2);
    queue.push(item2);
    REQUIRE(queue.size() == 2);
    MyQueueNode item3(3);
    queue.push(item3);
    REQUIRE(queue.size() == 3);

    REQUIRE(!queue.empty());

    int sum = 0;
    for (const auto& item : queue)
        sum += item.value;
    REQUIRE(sum == 6);

    MyQueueNode* node = queue.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(queue.size() == 2);

    node = queue.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(queue.size() == 1);

    queue.push(item2);
    REQUIRE(queue.size() == 2);
    queue.push(item1);
    REQUIRE(queue.size() == 3);

    queue.reverse();

    node = queue.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(queue.size() == 2);

    node = queue.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(queue.size() == 1);

    node = queue.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(queue.size() == 0);

    REQUIRE(queue.empty());
}
