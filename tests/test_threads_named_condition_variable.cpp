//
// Created by Ivan Shynkarenka on 04.10.2016.
//

#include "catch.hpp"

#include "threads/condition_variable.h"
#include "threads/named_condition_variable.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Named condition variable notify one", "[CppCommon][Threads]")
{
    int concurrency = 8;
    int result = 0;

    CriticalSection cs;
    ConditionVariable cv;

    // Named condition variable master
    NamedConditionVariable cv_master("named_condition_variable_test");

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv, &result]()
        {
            // Named condition variable slave
            NamedConditionVariable cv_slave("named_condition_variable_test");

            cs.Lock();
            ++result;
            cv.NotifyOne();
            cs.Unlock();
            cv_slave.Wait();
        }));
    }

    // Wait for all threads are started and waiting for notifications
    cs.Lock();
    cv.Wait(cs, [&result, concurrency]() { return (result == concurrency); });
    cs.Unlock();

    // Send one-thread notifications
    for (int i = 0; i < concurrency; ++i)
        cv_master.NotifyOne();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}

TEST_CASE("Named condition variable notify all", "[CppCommon][Threads]")
{
    int concurrency = 8;
    int result = 0;

    CriticalSection cs;
    ConditionVariable cv;

    // Named condition variable master
    NamedConditionVariable cv_master("named_condition_variable_test");

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&cs, &cv, &result]()
        {
            // Named condition variable slave
            NamedConditionVariable cv_slave("named_condition_variable_test");

            cs.Lock();
            ++result;
            cv.NotifyOne();
            cs.Unlock();
            cv_slave.Wait();
        }));
    }

    // Wait for all threads are started and waiting for notifications
    cs.Lock();
    cv.Wait(cs, [&result, concurrency]() { return (result == concurrency); });
    cs.Unlock();

    // Send all-threads notification
    cv_master.NotifyAll();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}
