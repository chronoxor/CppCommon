//
// Created by Ivan Shynkarenka on 17.08.2017
//

#include "catch.hpp"

#include "threads/seq_lock.h"
#include "threads/thread.h"

#include <thread>
#include <vector>

using namespace CppCommon;

struct Data
{
    int a;
    int b;
    int c;

    friend bool operator==(const Data& data1, const Data& data2)
    { return ((data1.a == data2.a) && (data1.b == data2.b) && (data1.c == data2.c)); }
};

TEST_CASE("SeqLock base", "[CppCommon][Threads]")
{
    Data data = { 0, 0, 0 };

    SeqLock<Data> lock = data;
    REQUIRE(lock.Read() == data);

    data = { 123, 456, 789 };
    lock.Write(data);
    REQUIRE(lock.Read() == data);
    REQUIRE(lock.Read() == data);

    data = { 987, 654, 321 };
    lock = data;
    REQUIRE(lock.Read() == data);
    REQUIRE(lock.Read() == data);
}

TEST_CASE("SeqLock random", "[CppCommon][Threads]")
{
    int items_to_produce = 1000000;
    int consumers_count = 4;

    SeqLock<Data> lock;

    // Start producer thread
    std::thread producer = std::thread([&lock, items_to_produce]()
    {
        for (int i = 0; i < items_to_produce; ++i)
        {
            // Use a sequential locker to produce the item
            lock.Write(Data{ i, i + 100, i + 200 });

            // Yield to another thread...
            Thread::Yield();
        }
    });

    // Start consumers threads
    std::vector<std::thread> consumers;
    for (int consumer = 0; consumer < consumers_count; ++consumer)
    {
        consumers.push_back(std::thread([&lock]()
        {
            Data data = lock.Read();
            REQUIRE(data.b == data.a + 100);
            REQUIRE(data.c == data.b + 100);

            // Yield to another thread...
            Thread::Yield();
        }));
    }

    // Wait for producer thread
    producer.join();

    // Wait for all consumers threads
    for (auto& consumer : consumers)
        consumer.join();
}
