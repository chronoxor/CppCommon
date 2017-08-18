/*!
    \file token_bucket.cpp
    \brief Token bucket rate limit algorithm implementation
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#include "algorithms/token_bucket.h"

#include "time/timestamp.h"

namespace CppCommon {

bool TokenBucket::Consume(uint64_t tokens)
{
    uint64_t now = Timestamp::nano();
    uint64_t delay = tokens * _time_per_token.load(std::memory_order_relaxed);
    uint64_t minTime = now - _time_per_burst.load(std::memory_order_relaxed);
    uint64_t oldTime = _time.load(std::memory_order_relaxed);
    uint64_t newTime = oldTime;

    // Previous consume performed long time ago... Shift the new time to the start of a new burst.
    if (minTime > oldTime)
        newTime = minTime;

    // Lock-free token consume loop
    for (;;)
    {
        // Consume tokens
        newTime += delay;

        // No more tokens left in the bucket
        if (newTime > now)
            return false;

        // Try to update the current time atomically
        if (_time.compare_exchange_weak(oldTime, newTime, std::memory_order_relaxed, std::memory_order_relaxed))
            return true;

        // Failed... Then retry consume tokens with a new time value
        newTime = oldTime;
    }
}

} // namespace CppCommon
