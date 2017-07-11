/*!
    \file containers_list.cpp
    \brief Intrusive list container example
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#include "containers/list.h"

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
    MyListNode item2(456);
    MyListNode item3(789);

    list.push_front(item1);
    list.push_back(item2);
    list.push_next(item2, item3);

    while (list)
        std::cout << "list.pop_front() = " << list.pop_front()->value << std::endl;

    return 0;
}
