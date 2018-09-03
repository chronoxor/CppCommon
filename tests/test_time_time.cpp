//
// Created by Ivan Shynkarenka on 13.07.2016
//

#include "test.h"

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
    REQUIRE((time1.utcstamp().total() == 1468408953123456789ll));

    Time time2(time1);
    UtcTime time3(time2.utcstamp());
    LocalTime time4(time2.localstamp());
    REQUIRE(time2 == time3);
    REQUIRE(time2 == time4);

    Time time5 = Time::epoch();
    REQUIRE(time5 == Time(1970, 1, 1, 0, 0, 0, 0, 0, 0));

    UtcTime time6;
    LocalTime time7(time6);
    UtcTime time8(time7);
    REQUIRE(time6 == time8);
    REQUIRE(time6 > Time::epoch());
    REQUIRE(time7 > Time::epoch());
    REQUIRE(std::abs((time6 - time7).hours()) < 24);

    // Compatibility with std::chrono
    UtcTime time9(std::chrono::system_clock::now() + std::chrono::milliseconds(10));
    std::this_thread::sleep_until(time9.chrono());
}
