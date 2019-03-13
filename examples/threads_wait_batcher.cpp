/*!
    \file threads_wait_batcher.cpp
    \brief Multiple producers / multiple consumers wait batcher example
    \author Ivan Shynkarenka
    \date 13.03.2019
    \copyright MIT License
*/

#include "threads/wait_batcher.h"

#include <iostream>
#include <iterator>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some string. Enter the empty string to exit..." << std::endl;

    // Create multiple producers / multiple consumers wait batcher
    CppCommon::WaitBatcher<char> batcher;

    // Start consumer thread
    auto consumer = std::thread([&batcher]()
    {
        std::vector<char> items;

        // Dequeue items until the batcher is closed
        while (batcher.Dequeue(items))
        {
            std::cout << "Your entered: ";
            std::copy(items.begin(), items.end(), std::ostream_iterator<char>(std::cout, ""));
            std::cout << std::endl;
        }
    });

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        // Enqueue the item or end produce
        if (!batcher.Enqueue(line.begin(), line.end()))
            break;

        if (line.empty())
        {
            // Close the wait batcher
            batcher.Close();
            break;
        }
    }

    // Wait for the consumer thread
    consumer.join();

    return 0;
}
