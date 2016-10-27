//
// Created by Ivan Shynkarenka on 03.10.2016.
//

#include "catch.hpp"

#include "threads/named_condition_variable.h"

#include <atomic>
#include <thread>

using namespace CppCommon;

TEST_CASE("Named condition variable notify one", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<int> result(0);

    // Named condition variables master
    NamedConditionVariable cv1_master("named_cv1_test");
    NamedConditionVariable cv2_master("named_cv2_test");

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&result]()
        {
            // Named condition variables slave
            NamedConditionVariable cv1_slave("named_cv1_test");
            NamedConditionVariable cv2_slave("named_cv2_test");

            ++result;
            cv1_slave.NotifyOne();
            cv2_slave.Wait();
        }));
    }

    // Wait for all threads are started and waiting for notifications
    cv1_master.Wait([&result, concurrency]() { return (result == concurrency); });

    // Send one-thread notifications
    for (int i = 0; i < concurrency; ++i)
        cv2_master.NotifyOne();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}

TEST_CASE("Named condition variable notify all", "[CppCommon][Threads]")
{
    int concurrency = 8;
    std::atomic<int> result(0);

    // Named condition variables master
    NamedConditionVariable cv1_master("named_cv1_test");
    NamedConditionVariable cv2_master("named_cv2_test");

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&result]()
        {
            // Named condition variables slave
            NamedConditionVariable cv1_slave("named_cv1_test");
            NamedConditionVariable cv2_slave("named_cv2_test");

            ++result;
            cv1_slave.NotifyOne();
            cv2_slave.Wait();
        }));
    }

    // Wait for all threads are started and waiting for notifications
    cv1_master.Wait([&result, concurrency]() { return (result == concurrency); });

    // Send all-threads notification
    cv2_master.NotifyAll();

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Check result
    REQUIRE(result == concurrency);
}
