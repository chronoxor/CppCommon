/*!
    \file intrusive_queue.cpp
    \brief Intrusive queue container example
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#include "intrusive/queue.h"

#include <iostream>

struct MyQueueNode : public CppCommon::Queue<MyQueueNode>::Node
{
    int value;
    MyQueueNode(int v) : value(v) {}
};

int main(int argc, char** argv)
{
    CppCommon::Queue<MyQueueNode> queue;

    MyQueueNode item1(123);
    MyQueueNode item2(456);
    MyQueueNode item3(789);

    queue.Push(item1);
    queue.Push(item2);
    queue.Push(item3);

    while (queue)
        std::cout << "queue.Pop() = " << queue.Pop()->value << std::endl;

    return 0;
}
