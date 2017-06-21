/*!
    \file intrusive_list.cpp
    \brief Intrusive list container example
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#include "intrusive/list.h"

#include <iostream>

struct MyListNode : public CppCommon::List<MyListNode>::Node
{
    int value;
    MyListNode(int v) : value(v) {}
};

int main(int argc, char** argv)
{
    CppCommon::List<MyListNode> list;

    MyListNode item1(123);
    list.PushFront(item1);
    MyListNode item2(456);
    list.PushBack(item2);
    MyListNode item3(789);
    list.PushNext(item2, item3);

    while (list)
        std::cout << "list.PopFront() = " << list.PopFront()->value << std::endl;

    return 0;
}
