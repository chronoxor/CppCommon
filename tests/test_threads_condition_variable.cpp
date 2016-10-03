//
// Created by Ivan Shynkarenka on 03.10.2016.
//

#include "catch.hpp"

#include "threads/condition_variable.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Condition variable notify one", "[CppCommon][Threads]")
{
    int concurrency = 8;
    int result1 = concurrency;
    int result2 = concurrency;

    CriticalSection cs;
    ConditionVariable cv1;
    ConditionVariable cv2;

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv1, &cv2, &result1, &result2]()
        {
            cs.Lock();
            --result1;
            cv1.NotifyOne();
            cv2.Wait(cs, [&result2]() { return (result2 == 0); });
            cs.Unlock();
        }));
    }

    // Send one-thread notifications
    for (int i = 0; i < concurrency; ++i)
    {
        cs.Lock();
        cv1.Wait(cs, [&result1]() { return (result1 == 0); });
        --result2;
        cv2.NotifyOne();
        cs.Unlock();
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(result1 == 0);
    REQUIRE(result2 == 0);
}

TEST_CASE("Condition variable notify all", "[CppCommon][Threads]")
{
    int concurrency = 8;
    int result1 = concurrency;
    int result2 = concurrency;

    CriticalSection cs;
    ConditionVariable cv1;
    ConditionVariable cv2;

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv1, &cv2, &result1, &result2]()
        {
            cs.Lock();
            --result1;
            cv1.NotifyOne();
            cv2.Wait(cs, [&result2]() { return (result2 == 0); });
            cs.Unlock();
        }));
    }

    // Send all-threads notifications
    for (int i = 0; i < concurrency; ++i)
    {
        cs.Lock();
        cv1.Wait(cs, [&result1]() { return (result1 == 0); });
        result2 = 0;
        cv2.NotifyAll();
        cs.Unlock();
    }

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check results
    REQUIRE(result1 == 0);
    REQUIRE(result2 == 0);
}
