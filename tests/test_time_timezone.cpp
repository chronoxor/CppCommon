//
// Created by Ivan Shynkarenka on 18.07.2016
//

#include "test.h"

#include "time/timezone.h"

using namespace CppCommon;

TEST_CASE("Timezone", "[CppCommon][Time]")
{
    Timezone timezone1("Test", Timespan::hours(-8), Timespan::hours(1));
    REQUIRE(timezone1.name() == "Test");
    REQUIRE(timezone1.offset() == Timespan::hours(-8));
    REQUIRE(timezone1.daylight() == Timespan::hours(1));
    REQUIRE(timezone1.total() == Timespan::hours(-7));

    Timezone timezone2 = Timezone::utc();
    REQUIRE(timezone2 == Timezone("GMT", Timespan::zero()));

    UtcTime utctime;
    LocalTime localtime(utctime);
    Timezone timezone3 = Timezone::local();
    REQUIRE(localtime == timezone3.Convert(utctime));
    REQUIRE(utctime == timezone3.Convert(localtime));

    Timezone timezone4 = Timezone::utc();
    Timezone timezone5 = Timezone::local();
    REQUIRE(std::abs((timezone4.total() - timezone5.total()).hours()) < 24);
}
