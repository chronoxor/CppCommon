/*!
    \file threads_wait_queue.cpp
    \brief Multiple producers / multiple consumers wait queue example
    \author Ivan Shynkarenka
    \date 05.10.2016
    \copyright MIT License
*/

#include "threads/wait_queue.h"

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some integer numbers. Enter '0' to exit..." << std::endl;

    // Create multiple producers / multiple consumers wait queue
    CppCommon::WaitQueue<int> queue;

    // Start consumer thread
    auto consumer = std::thread([&queue]()
    {
        int item;

        do
        {
            // Dequeue the item or end consume
            if (!queue.Dequeue(item))
                break;

            // Consume the item
            std::cout << "Your entered number: " << item << std::endl;
        } while (item != 0);
    });

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        int item = std::stoi(line);

        // Enqueue the item or end produce
        if (!queue.Enqueue(item))
            break;

        if (item == 0)
        {
            // Close the wait queue
            queue.Close();
            break;
        }
    }

    // Wait for the consumer thread
    consumer.join();

    return 0;
}
