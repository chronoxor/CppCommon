/*!
    \file string_format.cpp
    \brief Format string example
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

#include "string/format.h"

#include <iostream>

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

template <>
struct fmt::formatter<Date>
{
     constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Date& date, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}-{}-{}", date.year(), date.month(), date.day());
    }
};

#define SHOW(expression) std::cout << #expression << " = \"" << expression << "\""<< std::endl;

int main(int argc, char** argv)
{
    SHOW(CppCommon::format("argc: {}, argv: {}", argc, (void*)argv));
    SHOW(CppCommon::format("no arguments"));
    SHOW(CppCommon::format("{0}, {1}, {2}", -1, 0, 1));
    SHOW(CppCommon::format("{0}, {1}, {2}", 'a', 'b', 'c'));
    SHOW(CppCommon::format("{}, {}, {}", 'a', 'b', 'c'));
    SHOW(CppCommon::format("{2}, {1}, {0}", 'a', 'b', 'c'));
    SHOW(CppCommon::format("{0}{1}{0}", "abra", "cad"));
    SHOW(CppCommon::format("{:<30}", "left aligned"));
    SHOW(CppCommon::format("{:>30}", "right aligned"));
    SHOW(CppCommon::format("{:^30}", "centered"));
    SHOW(CppCommon::format("{:*^30}", "centered"));
    SHOW(CppCommon::format("{:+f}; {:+f}", 3.14, -3.14));
    SHOW(CppCommon::format("{: f}; {: f}", 3.14, -3.14));
    SHOW(CppCommon::format("{:-f}; {:-f}", 3.14, -3.14));
    SHOW(CppCommon::format("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42));
    SHOW(CppCommon::format("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42));
    SHOW(CppCommon::format("The date is {}", Date(2012, 12, 9)));
    SHOW(CppCommon::format("Elapsed time: {s:.2f} seconds", "s"_a = 1.23));
    return 0;
}
