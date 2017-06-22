//
// Created by Ivan Shynkarenka on 20.06.2017
//

#include "catch.hpp"

#include "intrusive/list.h"

using namespace CppCommon;

struct MyListNode : public CppCommon::List<MyListNode>::Node
{
    int value;
    MyListNode(int v) : value(v) {}
};

TEST_CASE("Intrusive list", "[CppCommon][Intrusive]")
{
    CppCommon::List<MyListNode> list;
    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);

    MyListNode item1(1);
    list.PushFront(item1);
    REQUIRE(list.size() == 1);
    MyListNode item2(2);
    list.PushBack(item2);
    REQUIRE(list.size() == 2);
    MyListNode item3(3);
    list.PushNext(item2, item3);
    REQUIRE(list.size() == 3);

    REQUIRE(!list.empty());

    int sum = 0;
    for (auto it : list)
        sum += it.value;
    REQUIRE(sum == 6);

    MyListNode* node = list.PopFront();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(list.size() == 2);

    node = list.PopBack();
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(list.size() == 1);

    list.PushPrev(item2, item3);
    REQUIRE(list.size() == 2);
    list.PushNext(item2, item1);
    REQUIRE(list.size() == 3);

    list.Reverse();

    node = list.PopPrev(item2);
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 1);
    REQUIRE(list.size() == 2);

    node = list.PopNext(item2);
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 3);
    REQUIRE(list.size() == 1);

    node = list.PopCurrent(item2);
    REQUIRE(node != nullptr);
    REQUIRE(node->value == 2);
    REQUIRE(list.size() == 0);

    REQUIRE(list.empty());
}
