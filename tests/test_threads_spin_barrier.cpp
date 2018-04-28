//
// Created by Ivan Shynkarenka on 17.03.2016
//

#include "test.h"

#include "threads/spin_barrier.h"
#include "threads/thread.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Spin barrier single thread", "[CppCommon][Threads]")
{
    SpinBarrier barrier(1);

    // Test Wait() method
    REQUIRE(barrier.Wait());
}

TEST_CASE("Spin barrier multiple threads", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<bool> failed(false);
    std::atomic<int> count(0);
    std::atomic<int> last(0);

    SpinBarrier barrier(concurrency);

    // Start some threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&barrier, &count, &last, &failed, concurrency, thread]()
        {
            // Increment threads counter
            ++count;

            // Sleep for a while...
            Thread::Sleep(thread * 10);

            // Wait for all other threads at the barrier
            if (barrier.Wait())
                ++last;

            // Check result in each thread
            if (count != concurrency)
                failed = true;
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(count == concurrency);
    REQUIRE(last == 1);
    REQUIRE(!failed);
}
