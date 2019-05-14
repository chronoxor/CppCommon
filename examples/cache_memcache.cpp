/*!
    \file cache_memcache.cpp
    \brief Memory cache example
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

#include "cache/memcache.h"
#include "threads/thread.h"

#include <iostream>

int main(int argc, char** argv)
{
    CppCommon::MemCache<std::string, int> cache;

    // Fill the memory cache
    cache.insert("123", 123);
    cache.insert("456", 456, CppCommon::Timespan::milliseconds(100));

    int result;

    // Get the memory cache values
    if (cache.find("123", result))
        std::cout << "Found: " << result << std::endl;
    if (cache.find("456", result))
        std::cout << "Found: " << result << std::endl;

    // Sleep for a while...
    CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(200));

    // Watchdog the memory cache to erase entries with timeout
    cache.watchdog();

    // Get the memory cache values
    if (cache.find("123", result))
        std::cout << "Found: " << result << std::endl;
    if (cache.find("456", result))
        std::cout << "Found: " << result << std::endl;

    return 0;
}
