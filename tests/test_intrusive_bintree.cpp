//
// Created by Ivan Shynkarenka on 22.06.2017
//

#include "catch.hpp"

#include "intrusive/bintree.h"
#include "intrusive/bintree_aa.h"
#include "intrusive/bintree_avl.h"
#include "intrusive/bintree_rb.h"
#include "intrusive/bintree_splay.h"

using namespace CppCommon;

struct MyBinTreeNode
{
    int value;

    MyBinTreeNode* parent;
    MyBinTreeNode* left;
    MyBinTreeNode* right;
    uint8_t balance;
    size_t level;
    bool rb;

    MyBinTreeNode(int v) : value(v) {}
    friend bool operator<(const MyBinTreeNode& node1, const MyBinTreeNode& node2)
    { return node1.value < node2.value; }
};

template <class TBinTree>
void test()
{
    TBinTree bintree;
    REQUIRE(bintree.empty());
    REQUIRE(bintree.size() == 0);

    MyBinTreeNode item1(1);
    MyBinTreeNode item2(2);
    MyBinTreeNode item3(3);
    MyBinTreeNode item4(4);
    MyBinTreeNode item5(5);
    MyBinTreeNode item6(6);
    MyBinTreeNode item7(7);
    MyBinTreeNode item8(8);
    MyBinTreeNode item9(9);

    bintree.Push(item6);
    REQUIRE(bintree.size() == 1);
    bintree.Push(item3);
    REQUIRE(bintree.size() == 2);
    bintree.Push(item7);
    REQUIRE(bintree.size() == 3);
    bintree.Push(item2);
    REQUIRE(bintree.size() == 4);
    bintree.Push(item8);
    REQUIRE(bintree.size() == 5);
    bintree.Push(item1);
    REQUIRE(bintree.size() == 6);
    bintree.Push(item4);
    REQUIRE(bintree.size() == 7);
    bintree.Push(item9);
    REQUIRE(bintree.size() == 8);
    bintree.Push(item5);
    REQUIRE(bintree.size() == 9);

    REQUIRE(!bintree.empty());

    REQUIRE(bintree.root()->value == 6);
    REQUIRE(bintree.lowest()->value == 1);
    REQUIRE(bintree.highest()->value == 9);

    int sum = 0;
    int prev = 0;
    for (auto it = bintree.begin(); it != bintree.end(); ++it)
    {
        REQUIRE(prev < it->value);
        prev = it->value;
        sum += it->value;
    }
    REQUIRE(sum == 45);

    sum = 0;
    prev = 10;
    for (auto it = bintree.rbegin(); it != bintree.rend(); ++it)
    {
        REQUIRE(prev > it->value);
        prev = it->value;
        sum += it->value;
    }
    REQUIRE(sum == 45);

    REQUIRE(!bintree.find(0));
    REQUIRE(bintree.find(1));
    REQUIRE(bintree.find(2));
    REQUIRE(bintree.find(3));
    REQUIRE(bintree.find(4));
    REQUIRE(bintree.find(5));
    REQUIRE(bintree.find(6));
    REQUIRE(bintree.find(7));
    REQUIRE(bintree.find(8));
    REQUIRE(bintree.find(9));
    REQUIRE(!bintree.find(10));

    REQUIRE(bintree.lower_bound(0)->value == 1);
    REQUIRE(bintree.lower_bound(1)->value == 1);
    REQUIRE(bintree.lower_bound(2)->value == 2);
    REQUIRE(bintree.lower_bound(3)->value == 3);
    REQUIRE(bintree.lower_bound(4)->value == 4);
    REQUIRE(bintree.lower_bound(5)->value == 5);
    REQUIRE(bintree.lower_bound(6)->value == 6);
    REQUIRE(bintree.lower_bound(7)->value == 7);
    REQUIRE(bintree.lower_bound(8)->value == 8);
    REQUIRE(bintree.lower_bound(9)->value == 9);
    REQUIRE(!bintree.lower_bound(10));

    REQUIRE(bintree.upper_bound(0)->value == 1);
    REQUIRE(bintree.upper_bound(1)->value == 2);
    REQUIRE(bintree.upper_bound(2)->value == 3);
    REQUIRE(bintree.upper_bound(3)->value == 4);
    REQUIRE(bintree.upper_bound(4)->value == 5);
    REQUIRE(bintree.upper_bound(5)->value == 6);
    REQUIRE(bintree.upper_bound(6)->value == 7);
    REQUIRE(bintree.upper_bound(7)->value == 8);
    REQUIRE(bintree.upper_bound(8)->value == 9);
    REQUIRE(!bintree.upper_bound(9));

    REQUIRE(!bintree.Pop(0));
    REQUIRE(!bintree.Pop(10));

    REQUIRE(bintree.Pop(1));
    REQUIRE(bintree.size() == 8);
    REQUIRE(bintree.Pop(3));
    REQUIRE(bintree.size() == 7);
    REQUIRE(bintree.Pop(6));
    REQUIRE(bintree.size() == 6);
    REQUIRE(bintree.Pop(9));
    REQUIRE(bintree.size() == 5);

    REQUIRE(bintree.lowest()->value == 2);
    REQUIRE(bintree.highest()->value == 8);

    REQUIRE(!bintree.find(0));
    REQUIRE(!bintree.find(1));
    REQUIRE(bintree.find(2));
    REQUIRE(!bintree.find(3));
    REQUIRE(bintree.find(4));
    REQUIRE(bintree.find(5));
    REQUIRE(!bintree.find(6));
    REQUIRE(bintree.find(7));
    REQUIRE(bintree.find(8));
    REQUIRE(!bintree.find(9));
    REQUIRE(!bintree.find(10));

    REQUIRE(bintree.lower_bound(0)->value == 2);
    REQUIRE(bintree.lower_bound(1)->value == 2);
    REQUIRE(bintree.lower_bound(2)->value == 2);
    REQUIRE(bintree.lower_bound(3)->value == 4);
    REQUIRE(bintree.lower_bound(4)->value == 4);
    REQUIRE(bintree.lower_bound(5)->value == 5);
    REQUIRE(bintree.lower_bound(6)->value == 7);
    REQUIRE(bintree.lower_bound(7)->value == 7);
    REQUIRE(bintree.lower_bound(8)->value == 8);
    REQUIRE(!bintree.lower_bound(9));
    REQUIRE(!bintree.lower_bound(10));

    REQUIRE(bintree.upper_bound(0)->value == 2);
    REQUIRE(bintree.upper_bound(1)->value == 2);
    REQUIRE(bintree.upper_bound(2)->value == 4);
    REQUIRE(bintree.upper_bound(3)->value == 4);
    REQUIRE(bintree.upper_bound(4)->value == 5);
    REQUIRE(bintree.upper_bound(5)->value == 7);
    REQUIRE(bintree.upper_bound(6)->value == 7);
    REQUIRE(bintree.upper_bound(7)->value == 8);
    REQUIRE(!bintree.upper_bound(8));
    REQUIRE(!bintree.upper_bound(9));

    REQUIRE(bintree.Pop(5));
    REQUIRE(bintree.size() == 4);
    REQUIRE(bintree.Pop(2));
    REQUIRE(bintree.size() == 3);
    REQUIRE(bintree.Pop(7));
    REQUIRE(bintree.size() == 2);
    REQUIRE(bintree.Pop(8));
    REQUIRE(bintree.size() == 1);
    REQUIRE(bintree.Pop(4));
    REQUIRE(bintree.size() == 0);

    REQUIRE(!bintree.root());
    REQUIRE(!bintree.lowest());
    REQUIRE(!bintree.highest());

    REQUIRE(bintree.empty());
}

TEST_CASE("Intrusive non balanced binary tree", "[CppCommon][Intrusive]")
{
    test<CppCommon::BinTree<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced A.Andersson binary tree", "[CppCommon][Intrusive]")
{
    test<CppCommon::BinTreeAA<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced AVL binary tree", "[CppCommon][Intrusive]")
{
    test<CppCommon::BinTreeAVL<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced Reb-Black binary tree", "[CppCommon][Intrusive]")
{
    test<CppCommon::BinTreeRB<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced Splay binary tree", "[CppCommon][Intrusive]")
{
    test<CppCommon::BinTreeSplay<MyBinTreeNode>>();
}
