//
// Created by Ivan Shynkarenka on 26.05.2016.
//

#include "catch.hpp"

#include "threads/named_critical_section.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Named critical section", "[CppCommon][Threads]")
{
    // Named critical section master
    NamedCriticalSection master("named_critical_section_test");

    // Test TryLock() method
    {
        // Named critical section slave
        NamedCriticalSection slave("named_critical_section_test");

        REQUIRE(slave.TryLock());
        slave.Unlock();
    }

    // Test Lock()/Unlock() methods
    {
        // Named critical section slave
        NamedCriticalSection slave("named_critical_section_test");

        slave.Lock();
        slave.Unlock();
    }
}

TEST_CASE("Named critical section locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 4;
    int crc = 0;

    // Named critical section master
    NamedCriticalSection lock("named_critical_section_test");

    // Caclulate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.push_back(std::thread([&crc, producer, items_to_produce, producers_count]()
        {
            // Named critical section slave
            NamedCriticalSection lock("named_critical_section_test");

            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
            {
                Locker<NamedCriticalSection> locker(lock);
                crc += (producer * items) + i;
            }
        }));
    }

    // Wait for all producers threads
    for (auto& producer : producers)
        producer.join();

    // Check result
    REQUIRE(crc == result);
}
