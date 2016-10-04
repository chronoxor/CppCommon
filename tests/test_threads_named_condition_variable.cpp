//
// Created by Ivan Shynkarenka on 04.10.2016.
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

    // Named condition variable masters
    NamedConditionVariable cv1_master("named_condition_variable_test1");
    NamedConditionVariable cv2_master("named_condition_variable_test2");

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&result]()
        {
            // Named condition variable slaves
            NamedConditionVariable cv1_slave("named_condition_variable_test1");
            NamedConditionVariable cv2_slave("named_condition_variable_test2");

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

    // Named condition variable masters
    NamedConditionVariable cv1_master("named_condition_variable_test1");
    NamedConditionVariable cv2_master("named_condition_variable_test2");

    // Start threads
    std::vector<std::thread> threads;
    for (int thread = 0; thread < concurrency; ++thread)
    {
        threads.push_back(std::thread([&result]()
        {
            // Named condition variable slaves
            NamedConditionVariable cv1_slave("named_condition_variable_test1");
            NamedConditionVariable cv2_slave("named_condition_variable_test2");

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
