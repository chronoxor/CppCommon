/*!
    \file threads_spsc_ring_queue.cpp
    \brief Single producer / single consumer wait-free ring queue example
    \author Ivan Shynkarenka
    \date 16.01.2016
    \copyright MIT License
*/

#include "threads/spsc_ring_queue.h"

#include <windows.h>
#include <debugapi.h>

#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    std::cout << "Please enter some integer numbers. Enter '0' to exit..." << std::endl;

    // Create single producer / single consumer wait-free ring queue
    CppCommon::SPSCRingQueue<int> queue(8);

    // Start consumer thread
    auto consumer = std::thread([&queue]()
    {
        int item;

		uint8_t prev = 127;
        do
        {
            // Dequeue using yield waiting strategy
            while (!queue.Dequeue(item))
                std::this_thread::yield();

			//OutputDebugStringA(("OUT: " + std::to_string(item) + "\n").c_str());

			// Consume items
			if (((item == 0) && (prev != 127)) || ((item > 0) && (item <= prev)))
					break;

    		prev = (uint8_t)item;
		} while (true);
    });

	while (true)
	{
		for (uint8_t i = 0; i < 128; ++i)
		{
			//OutputDebugStringA(("IN: " + std::to_string(i) + "\n").c_str());
			while (!queue.Enqueue(i))
				std::this_thread::yield();
		}
	}

    return 0;
}
