//
// Created by Ivan Shynkarenka on 15.05.2018
//

#include "test.h"

#include "string/string_utils.h"

using namespace CppCommon;

TEST_CASE("String utilities", "[CppCommon][String]")
{
    REQUIRE(StringUtils::ToLower('A') == 'a');
    REQUIRE(StringUtils::ToUpper('a') == 'A');
    REQUIRE(StringUtils::ToLower("aBcDeFg") == "abcdefg");
    REQUIRE(StringUtils::ToUpper("aBcDeFg") == "ABCDEFG");

    std::string str;

    str = "aBcDeFg";
    StringUtils::Lower(str);
    REQUIRE(str == "abcdefg");
    str = "aBcDeFg";
    StringUtils::Upper(str);
    REQUIRE(str == "ABCDEFG");

    REQUIRE(CppCommon::StringUtils::ToLTrim("    ") == "");
    REQUIRE(CppCommon::StringUtils::ToLTrim("  foobar  ") == "foobar  ");
    REQUIRE(CppCommon::StringUtils::ToLTrim("foobar  ") == "foobar  ");
    REQUIRE(CppCommon::StringUtils::ToRTrim("    ") == "");
    REQUIRE(CppCommon::StringUtils::ToRTrim("  foobar  ") == "  foobar");
    REQUIRE(CppCommon::StringUtils::ToRTrim("  foobar") == "  foobar");
    REQUIRE(CppCommon::StringUtils::ToTrim("    ") == "");
    REQUIRE(CppCommon::StringUtils::ToTrim("  foobar  ") == "foobar");
    REQUIRE(CppCommon::StringUtils::ToTrim("  foobar") == "foobar");
    REQUIRE(CppCommon::StringUtils::ToTrim("foobar  ") == "foobar");
    REQUIRE(CppCommon::StringUtils::ToTrim("foobar") == "foobar");

    str = "    ";
    REQUIRE(CppCommon::StringUtils::LTrim(str) == "");
    str = "  foobar  ";
    REQUIRE(CppCommon::StringUtils::LTrim(str) == "foobar  ");
    str = "foobar  ";
    REQUIRE(CppCommon::StringUtils::LTrim(str) == "foobar  ");
    str = "    ";
    REQUIRE(CppCommon::StringUtils::RTrim(str) == "");
    str = "  foobar  ";
    REQUIRE(CppCommon::StringUtils::RTrim(str) == "  foobar");
    str = "  foobar";
    REQUIRE(CppCommon::StringUtils::RTrim(str) == "  foobar");
    str = "    ";
    REQUIRE(CppCommon::StringUtils::Trim(str) == "");
    str = "  foobar  ";
    REQUIRE(CppCommon::StringUtils::Trim(str) == "foobar");
    str = "  foobar";
    REQUIRE(CppCommon::StringUtils::Trim(str) == "foobar");
    str = "foobar  ";
    REQUIRE(CppCommon::StringUtils::Trim(str) == "foobar");
    str = "foobar";
    REQUIRE(CppCommon::StringUtils::Trim(str) == "foobar");

    REQUIRE(!CppCommon::StringUtils::Contains("a foo a bar a baz", '!'));
    REQUIRE(CppCommon::StringUtils::Contains("a foo a bar a baz", 'z'));
    REQUIRE(CppCommon::StringUtils::Contains("a foo a bar a baz", "foo"));
    REQUIRE(!CppCommon::StringUtils::Contains("a foo a bar a baz", "foobar"));

    REQUIRE(CppCommon::StringUtils::CountAll("a foo a bar a baz", "test") == 0);
    REQUIRE(CppCommon::StringUtils::CountAll("a foo a bar a baz", "foo") == 1);
    REQUIRE(CppCommon::StringUtils::CountAll("a foo a bar a baz", "a ") == 3);

    str = "a foo a bar a baz";
    CppCommon::StringUtils::ReplaceFirst(str, "a ", "the ");
    REQUIRE(str == "the foo a bar a baz");

    str = "a foo a bar a baz";
    CppCommon::StringUtils::ReplaceLast(str, "a ", "the ");
    REQUIRE(str == "a foo a bar the baz");

    str = "a foo a bar a baz";
    CppCommon::StringUtils::ReplaceAll(str, "a ", "the ");
    REQUIRE(str == "the foo the bar the baz");

    REQUIRE(!StringUtils::StartsWith("CoolTest", "My"));
    REQUIRE(StringUtils::StartsWith("MyCoolTest", "My"));
    REQUIRE(!StringUtils::EndsWith("Cool", "Test"));
    REQUIRE(StringUtils::EndsWith("CoolTest", "Test"));

    REQUIRE(CppCommon::StringUtils::Join(CppCommon::StringUtils::Split("a foo a bar a baz", ' '), '+') == "a+foo+a+bar+a+baz");
    REQUIRE(CppCommon::StringUtils::Join(CppCommon::StringUtils::Split("a foo a bar a baz", "a "), "the ") == "the foo the bar the baz");

    REQUIRE(CppCommon::StringUtils::IsPatternMatch("Demo.*;Live.*", "DemoAccount"));
    REQUIRE(CppCommon::StringUtils::IsPatternMatch("Demo.*;Live.*", "LiveAccount"));
    REQUIRE(!CppCommon::StringUtils::IsPatternMatch("Demo.*;Live.*", "UnknownAccount"));
    REQUIRE(!CppCommon::StringUtils::IsPatternMatch("!Demo.*;!Live.*", "DemoAccount"));
    REQUIRE(!CppCommon::StringUtils::IsPatternMatch("!Demo.*;!Live.*", "LiveAccount"));
    REQUIRE(CppCommon::StringUtils::IsPatternMatch("!Demo.*;!Live.*", "UnknownAccount"));

    REQUIRE(CppCommon::StringUtils::FromString<bool>("true"));
    REQUIRE(CppCommon::StringUtils::FromString<bool>("yes"));
    REQUIRE(CppCommon::StringUtils::FromString<bool>("on"));
    REQUIRE(CppCommon::StringUtils::FromString<bool>("1"));
    REQUIRE(!CppCommon::StringUtils::FromString<bool>("false"));
    REQUIRE(!CppCommon::StringUtils::FromString<bool>("no"));
    REQUIRE(!CppCommon::StringUtils::FromString<bool>("off"));
    REQUIRE(!CppCommon::StringUtils::FromString<bool>("0"));
    REQUIRE(StringUtils::ToString('0') == "0");
    REQUIRE(StringUtils::ToString((int8_t)48) == "48");
    REQUIRE(StringUtils::ToString((uint8_t)49) == "49");
    REQUIRE(StringUtils::ToString(100) == "100");
    REQUIRE(StringUtils::ToString(123.456) == "123.456");
    REQUIRE(StringUtils::FromString<char>("0") == '0');
    REQUIRE(StringUtils::FromString<int8_t>("48") == '0');
    REQUIRE(StringUtils::FromString<uint8_t>("49") == '1');
    REQUIRE(StringUtils::FromString<int>("100") == 100);
    REQUIRE(StringUtils::FromString<double>("123.456") == 123.456);
}
