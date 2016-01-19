/*!
    \file wf_bounded_queue.cpp
    \brief Wait-free bounded queue example
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

#include <iostream>
#include <string>
#include <thread>

#include "threads/wf_bounded_queue.h"

int main(int argc, char** argv)
{
    std::cout << "Please write some integer numbers. Enter '0' to exit..." << std::endl;

    // Create wait-free bounded queue
    CppCommon::WFBoundedQueue<int> queue(1024);

    // Start consumer thread
    auto consumer = std::thread([&queue]()
    {
        int item;

        do
        {
            // Dequeue using yield waiting strategy
            while (!queue.Dequeue(item))
                std::this_thread::yield();

            // Consume the item
            std::cout << "Your entered number: " << item << std::endl;
        } while (item != 0);
    });

    // Perform text input.
    std::string line;
    while (getline(std::cin, line))
    {
        int item = std::stoi(line);

        // Enqueue using yield waiting strategy
        while (!queue.Enqueue(item))
            std::this_thread::yield();

        if (item == 0)
            break;
    }

    // Wait for the consumer thread
    consumer.join();

    return 0;
}
