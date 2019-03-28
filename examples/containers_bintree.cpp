/*!
    \file containers_bintree.cpp
    \brief Intrusive binary tree container example
    \author Ivan Shynkarenka
    \date 22.06.2017
    \copyright MIT License
*/

#include "containers/bintree.h"

#include <iostream>

struct MyBinTreeNode : public CppCommon::BinTree<MyBinTreeNode>::Node
{
    int value;
    MyBinTreeNode(int v) : value(v) {}
    friend bool operator<(const MyBinTreeNode& node1, const MyBinTreeNode& node2)
    { return node1.value < node2.value; }
};

int main(int argc, char** argv)
{
    assert("Duplicate node can not be inserted into the binary tree!");

    CppCommon::BinTree<MyBinTreeNode> bintree;

    MyBinTreeNode item1(1);
    MyBinTreeNode item2(2);
    MyBinTreeNode item3(3);
    MyBinTreeNode item4(4);
    MyBinTreeNode item5(5);
    MyBinTreeNode item6(6);
    MyBinTreeNode item7(7);
    MyBinTreeNode item8(8);
    MyBinTreeNode item9(9);

    bintree.insert(item6);
    bintree.insert(item3);
    bintree.insert(item7);
    bintree.insert(item2);
    bintree.insert(item8);
    bintree.insert(item1);
    bintree.insert(item4);
    bintree.insert(item9);
    bintree.insert(item5);

    std::cout << "bintree:" << std::endl;
    for (const auto& item : bintree)
        std::cout << item.value << std::endl;

    return 0;
}
