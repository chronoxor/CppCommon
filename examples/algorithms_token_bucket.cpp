/*!
    \file algorithms_token_bucket.cpp
    \brief Token bucket rate limit algorithm example
    \author Ivan Shynkarenka
    \date 16.08.2017
    \copyright MIT License
*/

#include "algorithms/token_bucket.h"

#include "threads/thread.h"
#include "time/timestamp.h"

#include <atomic>
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Press Enter to stop..." << std::endl;

    // Token bucket with one token per second rate and ten burst tokens
    CppCommon::TokenBucket tb(1, 10);

    std::atomic<bool> stop(false);
    std::thread worker = std::thread([&stop, &tb]()
    {
        while (!stop)
        {
            if (tb.Consume())
                std::cout << (CppCommon::UtcTimestamp().seconds() % 60) << " - Token consumed" << std::endl;
            else
                CppCommon::Thread::Yield();
        }
    });

    // Wait for input
    std::cin.get();

    // Stop the worker thread
    stop = true;

    // Wait for the worker thread
    worker.join();

    return 0;
}
