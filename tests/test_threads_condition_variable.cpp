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
    int result = 0;

    CriticalSection cs;
    ConditionVariable cv;

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv, &result]()
        {
            cs.Lock();
            cv.Wait(cs);
            ++result;
            cs.Unlock();
        }));
    }

    // Send one-thread notifications
    for (int i = 0; i < concurrency; ++i)
    {
        cs.Lock();
        cv.NotifyOne();
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
    ConditionVariable cv;

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv, &result]()
        {
            cs.Lock();
            cv.Wait(cs);
            ++result;
            cs.Unlock();
        }));
    }

    // Send all-thread notification
    cs.Lock();
    cv.NotifyAll();
    cs.Unlock();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}
