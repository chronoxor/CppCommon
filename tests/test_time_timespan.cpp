//
// Created by Ivan Shynkarenka on 11.07.2016.
//

#include "catch.hpp"

#include "threads/thread.h"
#include "time/timespan.h"

using namespace CppCommon;

TEST_CASE("Time span", "[CppCommon][Time]")
{
    TimeSpan span1(2558755123456789ll);
    TimeSpan span2(span1);

    REQUIRE(span1.days() == 29);
    REQUIRE((span1.hours() % 24) == 14);
    REQUIRE((span1.minutes() % 60) == 45);
    REQUIRE((span1.seconds() % 60) == 55);
    REQUIRE((span1.milliseconds() % 1000) == 123);
    REQUIRE((span1.microseconds() % 1000) == 456);
    REQUIRE((span1.nanoseconds() % 1000) == 789);

    REQUIRE(span2.days() == span1.days());
    REQUIRE(span2.hours() == (span1.days() * 24 + 14));
    REQUIRE(span2.minutes() == (span1.hours() * 60 + 45));
    REQUIRE(span2.seconds() == (span1.minutes() * 60 + 55));
    REQUIRE(span2.milliseconds() == (span1.seconds() * 1000 + 123));
    REQUIRE(span2.microseconds() == (span1.milliseconds() * 1000 + 456));
    REQUIRE(span2.nanoseconds() == (span1.microseconds() * 1000 + 789));

    // Compatibility with std::chrono
    TimeSpan span3(std::chrono::milliseconds(10));
    REQUIRE(span3.milliseconds() == 10);
    Thread::SleepFor(span3.chrono());
}
