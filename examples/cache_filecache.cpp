/*!
    \file cache_filecache.cpp
    \brief File cache example
    \author Ivan Shynkarenka
    \date 14.05.2019
    \copyright MIT License
*/

#include "cache/filecache.h"
#include "threads/thread.h"

#include <iostream>

int main(int argc, char** argv)
{
    CppCommon::FileCache cache;

    // Fill the file cache
    cache.insert("123", "123");
    cache.insert("456", "456", CppCommon::Timespan::milliseconds(100));

    std::pair<bool, std::string_view> result;

    // Get the memory cache values
    result = cache.find("123");
    if (result.first)
        std::cout << "Found: " << result.second << std::endl;
    result = cache.find("456");
    if (result.first)
        std::cout << "Found: " << result.second << std::endl;

    // Sleep for a while...
    CppCommon::Thread::SleepFor(CppCommon::Timespan::milliseconds(200));

    // Watchdog the memory cache to erase entries with timeout
    cache.watchdog();

    // Get the memory cache values
    result = cache.find("123");
    if (result.first)
        std::cout << "Found: " << result.second << std::endl;
    result = cache.find("456");
    if (result.first)
        std::cout << "Found: " << result.second << std::endl;

    return 0;
}
