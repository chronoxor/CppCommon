/*!
    \file containers_stack.cpp
    \brief Intrusive stack container example
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#include "containers/stack.h"

#include <iostream>

struct MyStackNode : public CppCommon::Stack<MyStackNode>::Node
{
    int value;

    explicit MyStackNode(int v) : value(v) {}
};

int main(int argc, char** argv)
{
    CppCommon::Stack<MyStackNode> stack;

    MyStackNode item1(123);
    MyStackNode item2(456);
    MyStackNode item3(789);

    stack.push(item1);
    stack.push(item2);
    stack.push(item3);

    while (stack)
        std::cout << "stack.pop() = " << stack.pop()->value << std::endl;

    return 0;
}
