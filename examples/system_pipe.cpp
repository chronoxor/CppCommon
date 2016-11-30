/*!
    \file system_pipe.cpp
    \brief Pipe example
    \author Ivan Shynkarenka
    \date 30.11.2016
    \copyright MIT License
*/

#include "system/pipe.h"

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some integer numbers. Enter '0' to exit..." << std::endl;

    // Create communication pipe
    CppCommon::Pipe pipe;

    // Start consumer thread
    auto consumer = std::thread([&pipe]()
    {
        int item;

        do
        {
            // Read the item form the pipe
            if (pipe.Read(&item, sizeof(item)) != sizeof(item))
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

        // Write the item into the pipe
        if (pipe.Write(&item, sizeof(item)) != sizeof(item))
            break;

        if (item == 0)
            break;
    }

    // Wait for the consumer thread
    consumer.join();

    return 0;
}
