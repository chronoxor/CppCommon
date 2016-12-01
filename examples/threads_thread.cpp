/*!
    \file threads_thread.cpp
    \brief Thread example
    \author Ivan Shynkarenka
    \date 08.08.2016
    \copyright MIT License
*/

#include "threads/thread.h"

#include "errors/exceptions_handler.h"
#include "system/stack_trace_manager.h"
#include "threads/critical_section.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char** argv)
{
    // Initialize stack trace manager of the current process
    CppCommon::StackTraceManager::Initialize();
    // Setup exceptions handler for the current process
    CppCommon::ExceptionsHandler::SetupProcess();

    std::cout << "Press Enter to stop..." << std::endl;

    CppCommon::CriticalSection lock;

    // Prepare thread CPU affinity bitsets (odd and even)
    std::bitset<64> affinity1(0x5555555555555555ull);
    std::bitset<64> affinity2(0xAAAAAAAAAAAAAAAAull);

    // Start some threads
    std::atomic<bool> stop(false);
    std::vector<std::thread> threads;
    for (int thread = 0; thread < 8; ++thread)
    {
        // Start thread with an exception handler registered
        std::thread thrd = CppCommon::Thread::Start([&lock, &stop, thread]()
        {
            while (!stop)
            {
                // Use locker with critical section to protect the output
                {
                    CppCommon::Locker<CppCommon::CriticalSection> locker(lock);
                    std::cout << "Thread Number: " << thread << ", Thread Id: " << CppCommon::Thread::CurrentThreadId() << ", Thread CPU affinity: " << CppCommon::Thread::CurrentThreadAffinity() << std::endl;
                }

                // Sleep for one second...
                CppCommon::Thread::Sleep(1000);
            }
        });

        // Set thread CPU affinity
        CppCommon::Thread::SetAffinity(thrd, ((thread % 2) == 0) ? affinity1 : affinity2);

        // Add the created thread to the collection
        threads.emplace_back(std::move(thrd));
    }

    // Wait for input
    std::cin.get();

    // Stop threads
    stop = true;

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    return 0;
}
