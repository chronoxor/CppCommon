//
// Created by Ivan Shynkarenka on 16.09.2016
//

#include "test.h"

#include "string/format.h"

using namespace CppCommon;

namespace {

class Date
{
public:
    Date(int year, int month, int day) : _year(year), _month(month), _day(day) {}

    int year() const { return _year; }
    int month() const { return _month; }
    int day() const { return _day; }

private:
    int _year, _month, _day;
};

} // namespace

template <>
struct fmt::formatter<Date>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Date& date, FormatContext& ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{}-{}-{}", date.year(), date.month(), date.day());
    }
};

TEST_CASE("Format", "[CppCommon][String]")
{
    REQUIRE(format("no arguments") == "no arguments");
    REQUIRE(format("{0}, {1}, {2}", -1, 0, 1) == "-1, 0, 1");
    REQUIRE(format("{0}, {1}, {2}", 'a', 'b', 'c') == "a, b, c");
    REQUIRE(format("{}, {}, {}", 'a', 'b', 'c') == "a, b, c");
    REQUIRE(format("{2}, {1}, {0}", 'a', 'b', 'c') == "c, b, a");
    REQUIRE(format("{0}{1}{0}", "abra", "cad") == "abracadabra");
    REQUIRE(format("{:<30}", "left aligned") == "left aligned                  ");
    REQUIRE(format("{:>30}", "right aligned") == "                 right aligned");
    REQUIRE(format("{:^30}", "centered") == "           centered           ");
    REQUIRE(format("{:*^30}", "centered") == "***********centered***********");
    REQUIRE(format("{:+f}; {:+f}", 3.14, -3.14) == "+3.140000; -3.140000");
    REQUIRE(format("{: f}; {: f}", 3.14, -3.14) == " 3.140000; -3.140000");
    REQUIRE(format("{:-f}; {:-f}", 3.14, -3.14) == "3.140000; -3.140000");
    REQUIRE(format("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42) == "int: 42;  hex: 2a;  oct: 52; bin: 101010");
    REQUIRE(format("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42) == "int: 42;  hex: 0x2a;  oct: 052;  bin: 0b101010");
    REQUIRE(format("The date is {}", Date(2012, 12, 9)) == "The date is 2012-12-9");
    REQUIRE(format("Elapsed time: {s:.2f} seconds", "s"_a = 1.23) == "Elapsed time: 1.23 seconds");
}
