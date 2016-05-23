//
// Created by Ivan Shynkarenka on 24.05.2016.
//

#include "catch.hpp"

#include "threads/named_event_auto_reset.h"
#include "threads/thread.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Named auto-reset event", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<int> count(0);

    // Named auto-reset event master
    NamedEventAutoReset event("named_auto_reset_event_test");

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&count, thread]()
        {
            // Named auto-reset event slave
            NamedEventAutoReset event("named_auto_reset_event_test");

            // Sleep for a while...
            Thread::SleepFor(std::chrono::milliseconds(thread * 10));

            // Wait for the event
            event.Wait();

            // Increment threads counter
            ++count;
        }));
    }

    // Allow threads to start
    Thread::SleepFor(std::chrono::milliseconds(100));

    // Signal the event for each thread that waits
    for (int thread = 0; thread < concurrency; ++thread)
        event.Signal();

    // Wait for all threads to complete
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(count == concurrency);
}
