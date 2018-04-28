/*!
    \file threads_seq_lock.cpp
    \brief Sequential lock synchronization primitive example
    \author Ivan Shynkarenka
    \date 17.08.2017
    \copyright MIT License
*/

#include "threads/seq_lock.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

struct Data
{
    int a;
    int b;
    int c;
};

int main(int argc, char** argv)
{
    CppCommon::SeqLock<Data> lock(Data{ 0, 0, 0 });

    std::cout << "Press Enter to stop..." << std::endl;

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < 4; ++thread)
    {
        threads.emplace_back([&lock, thread]()
        {
            for (;;)
            {
                Data data = lock.Read();
                if ((data.a == 100) && (data.b == 200) && (data.c == 300))
                {
                    std::cout << "Thread " << thread << " stopped!" << std::endl;
                    return;
                }
            }
        });
    }

    // Wait for input
    std::cin.get();

    // Stop threads
    lock = Data{ 100, 200, 300 };

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
