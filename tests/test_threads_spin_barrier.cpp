//
// Created by Ivan Shynkarenka on 17.03.2016.
//

#include "catch.hpp"

#include "threads/spin_barrier.h"
#include "threads/thread.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Spin barrier single thread", "[CppCommon][Threads]")
{
    SpinBarrier barrier(1);
    REQUIRE(barrier.wait());
}

TEST_CASE("Spin barrier multiple threads", "[CppCommon][Threads]")
{
    int threads = 8;
    std::atomic<bool> failed = false;
    std::atomic<int> count = 0;
    std::atomic<int> last = 0;
    SpinBarrier barrier(threads);

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < threads; ++producer)
    {
        producers.push_back(std::thread([&barrier, &count, &last, &failed, threads, producer]()
        {
            // Increment threads counter
            ++count;

            // Sleep for a while...
            Thread::Sleep(producer * 100);

            // Wait for all other threads at the barrier
            if (barrier.wait())
                ++last;

            // Check result in each thread
            if (count != threads)
                failed = true;
        }));
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check results
    REQUIRE(count == threads);
    REQUIRE(last == 1);
    REQUIRE(!failed);
}
