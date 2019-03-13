/*!
    \file threads_wait_ring.cpp
    \brief Multiple producers / multiple consumers wait ring example
    \author Ivan Shynkarenka
    \date 05.10.2016
    \copyright MIT License
*/

#include "threads/wait_ring.h"

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some integer numbers. Enter '0' to exit..." << std::endl;

    // Create multiple producers / multiple consumers wait ring
    CppCommon::WaitRing<int> ring(1024);

    // Start consumer thread
    auto consumer = std::thread([&ring]()
    {
        int item;

        do
        {
            // Dequeue the item or end consume
            if (!ring.Dequeue(item))
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
        if (!ring.Enqueue(item))
            break;

        if (item == 0)
        {
            // Close the wait ring
            ring.Close();
            break;
        }
    }

    // Wait for the consumer thread
    consumer.join();

    return 0;
}
