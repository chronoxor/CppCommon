/*!
    \file containers_queue.cpp
    \brief Intrusive queue container example
    \author Ivan Shynkarenka
    \date 20.06.2017
    \copyright MIT License
*/

#include "containers/queue.h"

#include <iostream>

struct MyQueueNode : public CppCommon::Queue<MyQueueNode>::Node
{
    int value;

    explicit MyQueueNode(int v) : value(v) {}
};

int main(int argc, char** argv)
{
    CppCommon::Queue<MyQueueNode> queue;

    MyQueueNode item1(123);
    MyQueueNode item2(456);
    MyQueueNode item3(789);

    queue.push(item1);
    queue.push(item2);
    queue.push(item3);

    while (queue)
        std::cout << "queue.pop() = " << queue.pop()->value << std::endl;

    return 0;
}
