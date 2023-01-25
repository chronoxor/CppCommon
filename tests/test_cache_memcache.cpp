//
// Created by Ivan Shynkarenka on 14.05.2019
//

#include "test.h"

#include "cache/memcache.h"
#include "threads/thread.h"

using namespace CppCommon;

TEST_CASE("Memory cache", "[CppCommon][Cache]")
{
    MemCache<std::string, int> cache;
    REQUIRE(cache.empty());
    REQUIRE(cache.size() == 0);

    // Fill the memory cache
    cache.insert("123", 123);
    cache.insert("456", 456, CppCommon::Timespan::milliseconds(100));
    cache.emplace("789", 789, CppCommon::Timespan::milliseconds(1000));

    int result = 0;
    Timestamp timeout;

    // Get the memory cache values
    REQUIRE(cache.find("123"));
    REQUIRE(cache.find("123", result));
    REQUIRE(result == 123);
    REQUIRE(cache.find("456"));
    REQUIRE(cache.find("456", result));
    REQUIRE(result == 456);
    REQUIRE(cache.find("789"));
    REQUIRE(cache.find("789", result, timeout));
    REQUIRE(result == 789);
    REQUIRE(timeout > UtcTimestamp());

    // Sleep for a while...
    Thread::SleepFor(Timespan::milliseconds(200));

    // Watchdog the memory cache to erase entries with timeout
    cache.watchdog();

    // Get the memory cache values
    REQUIRE(cache.find("123"));
    REQUIRE(cache.find("123", result));
    REQUIRE(result == 123);
    REQUIRE(!cache.find("456"));
    REQUIRE(cache.find("789"));
    REQUIRE(cache.find("789", result, timeout));
    REQUIRE(result == 789);
    REQUIRE(timeout > UtcTimestamp());

    // Remove the memory cache values
    REQUIRE(!cache.empty());
    REQUIRE(cache.size() == 2);
    REQUIRE(cache.remove("789"));
    REQUIRE(cache.size() == 1);

    // Clear the memory cache
    cache.clear();

    REQUIRE(cache.empty());
    REQUIRE(cache.size() == 0);
}
