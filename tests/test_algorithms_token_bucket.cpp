//
// Created by Ivan Shynkarenka on 16.08.2017
//

#include "test.h"

#include "algorithms/token_bucket.h"
#include "threads/thread.h"
#include "time/timestamp.h"

using namespace CppCommon;

TEST_CASE("Token bucket", "[CppCommon][Algorithms]")
{
    TokenBucket tb(1, 10);

    // Consume all tokens in the bucket
    REQUIRE(tb.Consume(10));

    // Failed to consume tokens at the current second
    REQUIRE(!tb.Consume());

    // Sleep for one second...
    Thread::SleepFor(Timespan::seconds(1));
    Thread::SleepFor(Timespan::milliseconds(1));

    REQUIRE(tb.Consume());
    REQUIRE(!tb.Consume());
    REQUIRE(!tb.Consume(1));
    REQUIRE(!tb.Consume(10));

    // Sleep for one second...
    Thread::SleepFor(Timespan::seconds(1));
    Thread::SleepFor(Timespan::milliseconds(1));

    REQUIRE(tb.Consume());
    REQUIRE(!tb.Consume());
    REQUIRE(!tb.Consume(1));
    REQUIRE(!tb.Consume(10));
}
