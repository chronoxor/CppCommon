//
// Created by Ivan Shynkarenka on 15.04.2016
//

#include "test.h"

#include "threads/named_mutex.h"

#include <thread>

using namespace CppCommon;

#if !defined(__APPLE__)

TEST_CASE("Named mutex locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 4;
    int crc = 0;

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Named mutex master
    NamedMutex lock_master("named_mutex_test");

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&crc, producer, items_to_produce, producers_count]()
        {
            // Named mutex slave
            NamedMutex lock_slave("named_mutex_test");

            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
            {
                Locker<NamedMutex> locker(lock_slave);
                crc += (producer * items) + i;
            }
        });
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check result
    REQUIRE(crc == result);
}

#endif
