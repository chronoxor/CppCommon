//
// Created by Ivan Shynkarenka on 22.06.2017
//

#include "test.h"

#include "containers/bintree.h"
#include "containers/bintree_aa.h"
#include "containers/bintree_avl.h"
#include "containers/bintree_rb.h"
#include "containers/bintree_splay.h"

using namespace CppCommon;

namespace {

struct MyBinTreeNode
{
    int value;

    MyBinTreeNode* parent;
    MyBinTreeNode* left;
    MyBinTreeNode* right;
    char balance;
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

    REQUIRE(bintree.insert(item6).second);
    REQUIRE(!bintree.insert(item6).second);
    REQUIRE(bintree.size() == 1);
    REQUIRE(bintree.insert(item3).second);
    REQUIRE(!bintree.insert(item3).second);
    REQUIRE(bintree.size() == 2);
    REQUIRE(bintree.insert(item7).second);
    REQUIRE(!bintree.insert(item7).second);
    REQUIRE(bintree.size() == 3);
    REQUIRE(bintree.insert(item2).second);
    REQUIRE(!bintree.insert(item2).second);
    REQUIRE(bintree.size() == 4);
    REQUIRE(bintree.insert(item8).second);
    REQUIRE(!bintree.insert(item8).second);
    REQUIRE(bintree.size() == 5);
    REQUIRE(bintree.insert(item1).second);
    REQUIRE(!bintree.insert(item1).second);
    REQUIRE(bintree.size() == 6);
    REQUIRE(bintree.insert(item4).second);
    REQUIRE(!bintree.insert(item4).second);
    REQUIRE(bintree.size() == 7);
    REQUIRE(bintree.insert(item9).second);
    REQUIRE(!bintree.insert(item9).second);
    REQUIRE(bintree.size() == 8);
    REQUIRE(bintree.insert(item5).second);
    REQUIRE(!bintree.insert(item5).second);
    REQUIRE(bintree.size() == 9);

    REQUIRE(!bintree.empty());

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

    REQUIRE(bintree.find(0) == bintree.end());
    REQUIRE(bintree.find(1) != bintree.end());
    REQUIRE(bintree.find(2) != bintree.end());
    REQUIRE(bintree.find(3) != bintree.end());
    REQUIRE(bintree.find(4) != bintree.end());
    REQUIRE(bintree.find(5) != bintree.end());
    REQUIRE(bintree.find(6) != bintree.end());
    REQUIRE(bintree.find(7) != bintree.end());
    REQUIRE(bintree.find(8) != bintree.end());
    REQUIRE(bintree.find(9) != bintree.end());
    REQUIRE(bintree.find(10) == bintree.end());

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
    REQUIRE(bintree.lower_bound(10) == bintree.end());

    REQUIRE(bintree.upper_bound(0)->value == 1);
    REQUIRE(bintree.upper_bound(1)->value == 2);
    REQUIRE(bintree.upper_bound(2)->value == 3);
    REQUIRE(bintree.upper_bound(3)->value == 4);
    REQUIRE(bintree.upper_bound(4)->value == 5);
    REQUIRE(bintree.upper_bound(5)->value == 6);
    REQUIRE(bintree.upper_bound(6)->value == 7);
    REQUIRE(bintree.upper_bound(7)->value == 8);
    REQUIRE(bintree.upper_bound(8)->value == 9);
    REQUIRE(bintree.upper_bound(9) == bintree.end());

    REQUIRE(bintree.erase(0) == nullptr);
    REQUIRE(bintree.erase(10) == nullptr);

    REQUIRE(bintree.erase(1) != nullptr);
    REQUIRE(bintree.size() == 8);
    REQUIRE(bintree.erase(3) != nullptr);
    REQUIRE(bintree.size() == 7);
    REQUIRE(bintree.erase(6) != nullptr);
    REQUIRE(bintree.size() == 6);
    REQUIRE(bintree.erase(9) != nullptr);
    REQUIRE(bintree.size() == 5);

    REQUIRE(bintree.lowest()->value == 2);
    REQUIRE(bintree.highest()->value == 8);

    REQUIRE(bintree.find(0) == bintree.end());
    REQUIRE(bintree.find(1) == bintree.end());
    REQUIRE(bintree.find(2) != bintree.end());
    REQUIRE(bintree.find(3) == bintree.end());
    REQUIRE(bintree.find(4) != bintree.end());
    REQUIRE(bintree.find(5) != bintree.end());
    REQUIRE(bintree.find(6) == bintree.end());
    REQUIRE(bintree.find(7) != bintree.end());
    REQUIRE(bintree.find(8) != bintree.end());
    REQUIRE(bintree.find(9) == bintree.end());
    REQUIRE(bintree.find(10) == bintree.end());

    REQUIRE(bintree.lower_bound(0)->value == 2);
    REQUIRE(bintree.lower_bound(1)->value == 2);
    REQUIRE(bintree.lower_bound(2)->value == 2);
    REQUIRE(bintree.lower_bound(3)->value == 4);
    REQUIRE(bintree.lower_bound(4)->value == 4);
    REQUIRE(bintree.lower_bound(5)->value == 5);
    REQUIRE(bintree.lower_bound(6)->value == 7);
    REQUIRE(bintree.lower_bound(7)->value == 7);
    REQUIRE(bintree.lower_bound(8)->value == 8);
    REQUIRE(bintree.lower_bound(9) == bintree.end());
    REQUIRE(bintree.lower_bound(10) == bintree.end());

    REQUIRE(bintree.upper_bound(0)->value == 2);
    REQUIRE(bintree.upper_bound(1)->value == 2);
    REQUIRE(bintree.upper_bound(2)->value == 4);
    REQUIRE(bintree.upper_bound(3)->value == 4);
    REQUIRE(bintree.upper_bound(4)->value == 5);
    REQUIRE(bintree.upper_bound(5)->value == 7);
    REQUIRE(bintree.upper_bound(6)->value == 7);
    REQUIRE(bintree.upper_bound(7)->value == 8);
    REQUIRE(bintree.upper_bound(8) == bintree.end());
    REQUIRE(bintree.upper_bound(9) == bintree.end());

    REQUIRE(bintree.erase(5) != nullptr);
    REQUIRE(bintree.size() == 4);
    REQUIRE(bintree.erase(2) != nullptr);
    REQUIRE(bintree.size() == 3);
    REQUIRE(bintree.erase(7) != nullptr);
    REQUIRE(bintree.size() == 2);
    REQUIRE(bintree.erase(8) != nullptr);
    REQUIRE(bintree.size() == 1);
    REQUIRE(bintree.erase(4) != nullptr);
    REQUIRE(bintree.size() == 0);

    REQUIRE(!bintree.root());
    REQUIRE(!bintree.lowest());
    REQUIRE(!bintree.highest());

    REQUIRE(bintree.empty());
}

} // namespace

TEST_CASE("Intrusive non balanced binary tree", "[CppCommon][Containers]")
{
    test<BinTree<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced A.Andersson binary tree", "[CppCommon][Containers]")
{
    test<BinTreeAA<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced AVL binary tree", "[CppCommon][Containers]")
{
    test<BinTreeAVL<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced Reb-Black binary tree", "[CppCommon][Containers]")
{
    test<BinTreeRB<MyBinTreeNode>>();
}

TEST_CASE("Intrusive balanced Splay binary tree", "[CppCommon][Containers]")
{
    test<BinTreeSplay<MyBinTreeNode>>();
}
