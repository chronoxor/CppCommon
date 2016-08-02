/*!
    \file threads_spsc_ring_buffer.cpp
    \brief Single producer / single consumer wait-free ring buffer example
    \author Ivan Shynkarenka
    \date 16.01.2016
    \copyright MIT License
*/

#include "threads/spsc_ring_buffer.h"

#include <windows.h>
#include <debugapi.h>

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some text. Enter '#' to exit..." << std::endl;

    // Create single producer / single consumer wait-free ring buffer
    CppCommon::SPSCRingBuffer buffer(1024);

    // Start consumer thread
    auto consumer = std::thread([&buffer]()
    {
        // Use local buffer for batch processing
        const size_t local_capacity = 1024;
        char local_buffer[local_capacity];
        size_t local_size;

		uint8_t prev = 127;
        do
        {
            // Dequeue with yield waiting strategy
            while (!buffer.Dequeue(local_buffer, local_size = local_capacity))
                std::this_thread::yield();

            // Consume items
			for (int i = 0; i < local_size; ++i)
			{
				//OutputDebugStringA(("OUT: " + std::to_string(local_buffer[i]) + "\n").c_str());
				if (((local_buffer[i] == 0) && (prev != 127)) || ((local_buffer[i] > 0) && (local_buffer[i] <= prev)))
					break;

			    prev = local_buffer[i];
			}
        } while (true);
    });

	while (true)
	{
		for (uint8_t i = 0; i < 128; ++i)
		{
			//OutputDebugStringA(("IN: " + std::to_string(i) + "\n").c_str());
			while (!buffer.Enqueue(&i, sizeof(uint8_t)))
				std::this_thread::yield();
		}
	}

    return 0;
}
