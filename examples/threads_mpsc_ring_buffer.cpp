/*!
    \file threads_mpsc_ring_buffer.cpp
    \brief Multiple producers / single consumer wait-free ring buffer example
    \author Ivan Shynkarenka
    \date 26 .01.2016
    \copyright MIT License
*/

#include "threads/mpsc_ring_buffer.h"

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some text. Enter '#' to exit..." << std::endl;

    // Create multiple producers / single consumer wait-free ring buffer
    CppCommon::MPSCRingBuffer buffer(1024);

    // Start consumer thread
    auto consumer = std::thread([&buffer]()
    {
        // Use local buffer for batch processing
        const size_t local_capacity = 1024;
        char local_buffer[local_capacity];
        size_t local_size;

        do
        {
            // Dequeue with yield waiting strategy
            while (!buffer.Dequeue(local_buffer, local_size = local_capacity))
                std::this_thread::yield();

            // Consume items
            std::cout << "Your entered: ";
            std::cout.write(local_buffer, local_size);
            std::cout << std::endl;
        } while (memchr(local_buffer, '#', local_size) == nullptr);
    });

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        // Enqueue using yield waiting strategy
        while (!buffer.Enqueue(line.c_str(), line.size()))
            std::this_thread::yield();

        if (line.find('#') != std::string::npos)
            break;
    }

    // Wait for the consumer thread
    consumer.join();

    return 0;
}
