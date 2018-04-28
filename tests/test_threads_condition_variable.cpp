//
// Created by Ivan Shynkarenka on 03.10.2016
//

#include "test.h"

#include "threads/condition_variable.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Condition variable notify one", "[CppCommon][Threads]")
{
    int concurrency = 8;
    int result = 0;

    CriticalSection cs;
    ConditionVariable cv1;
    ConditionVariable cv2;

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&cs, &cv1, &cv2, &result]()
        {
            cs.Lock();
            ++result;
            cv1.NotifyOne();
            cv2.Wait(cs);
            cs.Unlock();
        });
    }

    // Wait for all threads are started and waiting for notifications
    cs.Lock();
    cv1.Wait(cs, [&result, concurrency]() { return (result == concurrency); });
    cs.Unlock();

    // Send one-thread notifications
    for (int i = 0; i < concurrency; ++i)
    {
        cs.Lock();
        cv2.NotifyOne();
        cs.Unlock();
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}

TEST_CASE("Condition variable notify all", "[CppCommon][Threads]")
{
    int concurrency = 8;
    int result = 0;

    CriticalSection cs;
    ConditionVariable cv1;
    ConditionVariable cv2;

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.emplace_back([&cs, &cv1, &cv2, &result]()
        {
            cs.Lock();
            ++result;
            cv1.NotifyOne();
            cv2.Wait(cs);
            cs.Unlock();
        });
    }

    // Wait for all threads are started and waiting for notifications
    cs.Lock();
    cv1.Wait(cs, [&result, concurrency]() { return (result == concurrency); });
    cs.Unlock();

    // Send all-threads notification
    cs.Lock();
    cv2.NotifyAll();
    cs.Unlock();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}
