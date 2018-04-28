//
// Created by Ivan Shynkarenka on 04.04.2016
//

#include "test.h"

#include "threads/mutex.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Mutex", "[CppCommon][Threads]")
{
    Mutex lock;

    // Test TryLock() method
    REQUIRE(lock.TryLock());
    lock.Unlock();

    // Test Lock()/Unlock() methods
    lock.Lock();
    lock.Unlock();
}

TEST_CASE("Mutex locker", "[CppCommon][Threads]")
{
    int items_to_produce = 10000;
    int producers_count = 4;
    int crc = 0;

    Mutex lock;

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producers threads
    std::vector<std::thread> producers;
    for (int producer = 0; producer < producers_count; ++producer)
    {
        producers.emplace_back([&lock, &crc, producer, items_to_produce, producers_count]()
        {
            int items = (items_to_produce / producers_count);
            for (int i = 0; i < items; ++i)
            {
                Locker<Mutex> locker(lock);
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
