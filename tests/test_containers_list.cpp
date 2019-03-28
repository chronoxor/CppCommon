//
// Created by Ivan Shynkarenka on 20.06.2017
//

#include "test.h"

#include "containers/list.h"

using namespace CppCommon;

namespace {

struct MyListNode : public List<MyListNode>::Node
{
    int value;

    explicit MyListNode(int v) : value(v) {}
};

} // namespace

TEST_CASE("Intrusive list", "[CppCommon][Containers]")
{
    List<MyListNode> list;
    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);

    MyListNode item1(1);
    list.push_front(item1);
    REQUIRE(list.size() == 1);
    MyListNode item2(2);
    list.push_back(item2);
    REQUIRE(list.size() == 2);
    MyListNode item3(3);
    list.push_next(item2, item3);
    REQUIRE(list.size() == 3);

    REQUIRE(!list.empty());

    int sum = 0;
    for (const auto& item : list)
        sum += item.value;
    REQUIRE(sum == 6);

    MyListNode* node = list.pop_front();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(list.size() == 2);

    node = list.pop_back();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(list.size() == 1);

    list.push_prev(item2, item3);
    REQUIRE(list.size() == 2);
    list.push_next(item2, item1);
    REQUIRE(list.size() == 3);

    list.reverse();

    node = list.pop_prev(item2);
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(list.size() == 2);

    node = list.pop_next(item2);
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(list.size() == 1);

    node = list.pop_current(item2);
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(list.size() == 0);

    REQUIRE(list.empty());
}
