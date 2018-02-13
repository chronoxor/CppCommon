//
// Created by Ivan Shynkarenka on 30.11.2016
//

#include "test.h"

#include "system/pipe.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Pipe", "[CppCommon][System]")
{
    Pipe pipe;

    for (int i = 0; i < 1000; ++i)
    {
        int item = i;
        REQUIRE(pipe.Write(&item, sizeof(item)) == sizeof(item));
        REQUIRE(pipe.Read(&item, sizeof(item)) == sizeof(item));
        REQUIRE(item == i);
    }
}

TEST_CASE("Pipe threads", "[CppCommon][System]")
{
    int items_to_produce = 10000;
    int crc = 0;

    Pipe pipe;

    // Calculate result value
    int result = 0;
    for (int i = 0; i < items_to_produce; ++i)
        result += i;

    // Start producer thread
    auto producer = std::thread([&pipe, items_to_produce]()
    {
        for (int i = 0; i < items_to_produce; ++i)
        {
            int item = i;
            if (pipe.Write(&item, sizeof(item)) != sizeof(item))
                break;
        }
    });

    // Consume items
    for (int i = 0; i < items_to_produce; ++i)
    {
        int item;
        if (pipe.Read(&item, sizeof(item)) != sizeof(item))
            break;
        crc += item;
    }

    // Wait for producer thread
    producer.join();

    // Check result
    REQUIRE(crc == result);
}
