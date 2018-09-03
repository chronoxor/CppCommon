//
// Created by Ivan Shynkarenka on 11.07.2016
//

#include "test.h"

#include "time/timespan.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Timespan", "[CppCommon][Time]")
{
    Timespan span1 = Timespan::days(29) +
                     Timespan::hours(14) +
                     Timespan::minutes(45) +
                     Timespan::seconds(55) +
                     Timespan::milliseconds(123) +
                     Timespan::microseconds(456) +
                     Timespan::nanoseconds(789);
    Timespan span2(span1);

    REQUIRE(span1.total() == 2558755123456789ll);
    REQUIRE(span1.days() == 29);
    REQUIRE((span1.hours() % 24) == 14);
    REQUIRE((span1.minutes() % 60) == 45);
    REQUIRE((span1.seconds() % 60) == 55);
    REQUIRE((span1.milliseconds() % 1000) == 123);
    REQUIRE((span1.microseconds() % 1000) == 456);
    REQUIRE((span1.nanoseconds() % 1000) == 789);

    REQUIRE(span2.total() == span1.total());
    REQUIRE(span2.days() == span1.days());
    REQUIRE(span2.hours() == (span1.days() * 24 + 14));
    REQUIRE(span2.minutes() == (span1.hours() * 60 + 45));
    REQUIRE(span2.seconds() == (span1.minutes() * 60 + 55));
    REQUIRE(span2.milliseconds() == (span1.seconds() * 1000 + 123));
    REQUIRE(span2.microseconds() == (span1.milliseconds() * 1000 + 456));
    REQUIRE(span2.nanoseconds() == (span1.microseconds() * 1000 + 789));

    // Compatibility with std::chrono
    Timespan span3(std::chrono::milliseconds(10));
    REQUIRE(span3.milliseconds() == 10);
    std::this_thread::sleep_for(span3.chrono());
}
