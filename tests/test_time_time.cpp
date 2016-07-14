//
// Created by Ivan Shynkarenka on 13.07.2016.
//

#include "catch.hpp"

#include "time/time.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Time", "[CppCommon][Time]")
{
    Time time1(2016, 7, 13, 11, 22, 33, 123, 456, 789);
    REQUIRE(time1.year() == 2016);
    REQUIRE(time1.month() == 7);
    REQUIRE(time1.day() == 13);
    REQUIRE(time1.hour() == 11);
    REQUIRE(time1.minute() == 22);
    REQUIRE(time1.second() == 33);
    REQUIRE(time1.millisecond() == 123);
    REQUIRE(time1.microsecond() == 456);
    REQUIRE(time1.nanosecond() == 789);
    REQUIRE(time1.timestamp().total() == 1468398153123456789ll);

    Time time2(time1);
    UtcTime time3(time2.timestamp());
    REQUIRE(time2 == time3);

    Time time4 = Time::epoch();
    REQUIRE(time4 == Time(1970, 1, 1, 0, 0, 0, 0, 0, 0));

    UtcTime time5 = UtcTime();
    LocalTime time6 = LocalTime();
    REQUIRE(time5 > Time::epoch());
    REQUIRE(time6 > Time::epoch());
    REQUIRE(std::abs((time5 - time6).hours()) < 24);

    // Compatibility with std::chrono
    UtcTime time7 = UtcTime::chrono(std::chrono::system_clock::now() + std::chrono::milliseconds(10));
    std::this_thread::sleep_until(time7.chrono());
}
