//
// Created by Ivan Shynkarenka on 24.05.2016
//

#include "test.h"

#include "threads/named_event_manual_reset.h"
#include "threads/thread.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

#if !defined(__APPLE__)

TEST_CASE("Named manual-reset event", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<int> count(0);

    // Named manual-reset event master
    NamedEventManualReset event_master("named_manual_event_test");

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&count, thread]()
        {
            // Named manual-reset event slave
            NamedEventManualReset event_slave("named_manual_event_test");

            // Sleep for a while...
            Thread::Sleep(thread * 10);

            // Wait for the event
            event_slave.Wait();

            // Increment threads counter
            ++count;
        });
    }

    // Allow threads to start
    Thread::Sleep(100);

    // Signal the event
    event_master.Signal();

    // Wait for all threads to complete
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(count == concurrency);
}

#endif
