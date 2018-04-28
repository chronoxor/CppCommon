//
// Created by Ivan Shynkarenka on 25.03.2016
//

#include "test.h"

#include "threads/latch.h"
#include "threads/thread.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Latch single thread", "[CppCommon][Threads]")
{
    Latch latch(3);

    // Test CountDown()/TryWait()/Wait() methods
    REQUIRE(!latch.TryWait());
    latch.CountDown();
    REQUIRE(!latch.TryWait());
    latch.CountDown();
    REQUIRE(!latch.TryWait());
    latch.CountDown();
    latch.Wait();
    REQUIRE(latch.TryWait());

    // Test Rest() method
    latch.Reset(1);

    REQUIRE(!latch.TryWait());
    latch.CountDown();
    latch.Wait();
    REQUIRE(latch.TryWait());
}

TEST_CASE("Latch one thread to wait for multiple threads", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<int> count(0);
    Latch latch(concurrency);

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&latch, &count, thread]()
        {
            // Increment threads counter
            ++count;

            // Sleep for a while...
            Thread::Sleep(thread * 10);

            // Count down the latch
            latch.CountDown();
        });
    }

    // Wait until work is done
    latch.Wait();

    // Wait for all threads to complete
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(count == concurrency);
}

TEST_CASE("Latch multiple threads to wait for the one initialization thread", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<int> count(0);

    Latch latch(1);

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&latch, &count, thread]()
        {
            // Wait for the latch
            latch.Wait();

            // Increment threads counter
            ++count;

            // Sleep for a while...
            Thread::Sleep(thread * 100);
        });
    }

    // Perform some initialization
    Thread::Sleep(100);

    // Threads can now start processing
    latch.CountDown();

    // Wait for all threads to complete
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(count == concurrency);
}
