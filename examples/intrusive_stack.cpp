/*!
    \file intrusive_stack.cpp
    \brief Intrusive stack container example
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#include "intrusive/stack.h"

#include <iostream>

struct MyStackNode : public CppCommon::Stack<MyStackNode>::Node
{
    int value;
    MyStackNode(int v) : value(v) {}
};

int main(int argc, char** argv)
{
    CppCommon::Stack<MyStackNode> stack;

    MyStackNode item1(123);
    MyStackNode item2(456);
    MyStackNode item3(789);

    stack.Push(item1);
    stack.Push(item2);
    stack.Push(item3);

    while (stack)
        std::cout << "stack.Pop() = " << stack.Pop()->value << std::endl;

    return 0;
}
