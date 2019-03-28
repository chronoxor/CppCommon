//
// Created by Ivan Shynkarenka on 20.06.2017
//

#include "test.h"

#include "containers/stack.h"

using namespace CppCommon;

namespace {

struct MyStackNode : public Stack<MyStackNode>::Node
{
    int value;

    explicit MyStackNode(int v) : value(v) {}
};

} // namespace

TEST_CASE("Intrusive stack", "[CppCommon][Containers]")
{
    Stack<MyStackNode> stack;
    REQUIRE(stack.empty());
    REQUIRE(stack.size() == 0);

    MyStackNode item1(1);
    stack.push(item1);
    REQUIRE(stack.size() == 1);
    MyStackNode item2(2);
    stack.push(item2);
    REQUIRE(stack.size() == 2);
    MyStackNode item3(3);
    stack.push(item3);
    REQUIRE(stack.size() == 3);

    REQUIRE(!stack.empty());

    int sum = 0;
    for (const auto& item : stack)
        sum += item.value;
    REQUIRE(sum == 6);

    MyStackNode* node = stack.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(stack.size() == 2);

    node = stack.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(stack.size() == 1);

    stack.push(item2);
    REQUIRE(stack.size() == 2);
    stack.push(item3);
    REQUIRE(stack.size() == 3);

    stack.reverse();

    node = stack.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(stack.size() == 2);

    node = stack.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(stack.size() == 1);

    node = stack.pop();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(stack.size() == 0);

    REQUIRE(stack.empty());
}
