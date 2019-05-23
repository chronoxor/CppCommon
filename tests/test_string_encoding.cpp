//
// Created by Ivan Shynkarenka on 13.08.2016
//

#include "test.h"

#include "string/encoding.h"

using namespace CppCommon;

namespace {

void test(const std::string& utf8, const std::u16string& utf16, const std::u32string& utf32)
{
    REQUIRE(Encoding::UTF8toUTF16(utf8) == utf16);
    REQUIRE(Encoding::UTF8toUTF32(utf8) == utf32);
    REQUIRE(Encoding::UTF16toUTF8(utf16) == utf8);
    REQUIRE(Encoding::UTF16toUTF32(utf16) == utf32);
    REQUIRE(Encoding::UTF32toUTF8(utf32) == utf8);
    REQUIRE(Encoding::UTF32toUTF16(utf32) == utf16);

    std::wstring wstr = Encoding::FromUTF8(utf8);
    std::string str = Encoding::ToUTF8(wstr);
    REQUIRE(str == utf8);
}

} // namespace

TEST_CASE("Encoding", "[CppCommon][String]")
{
    test("\x61", u"\x0061", U"\x00000061");
    test("\xEF\xBD\x81", u"\xFF41", U"\x0000FF41");
    test("\xC4\x8D", u"\x010D", U"\x010D");
    test("\x63\xCC\x8C", u"\x0063\x030C", U"\x00000063\x0000030C");
    test("\xC4\xB3", u"\x0133", U"\x00000133");
    test("\x69\x6A", u"\x0069\x006A", U"\x00000069\x0000006A");
    test("\xCE\xA9", u"\x03A9", U"\x000003A9");
    test("\xE2\x84\xA6", u"\x2126", U"\x00002126");
    test("\xF0\x9D\x93\x83", u"\xD835\xDCC3", U"\x0001D4C3");
}

TEST_CASE("Base64 Encoding", "[CppCommon][String]")
{
    REQUIRE(Encoding::Base64Encode("Sample Base64 encoding: ~`'\"!?@#$%^&*(){}[]<>,.:;-+=_|/\\") == "U2FtcGxlIEJhc2U2NCBlbmNvZGluZzogfmAnXCIhP0AjJCVeJiooKXt9W108PiwuOjstKz1ffC9cXA==");
    REQUIRE(Encoding::Base64Decode("U2FtcGxlIEJhc2U2NCBlbmNvZGluZzogfmAnXCIhP0AjJCVeJiooKXt9W108PiwuOjstKz1ffC9cXA==") == "Sample Base64 encoding: ~`'\"!?@#$%^&*(){}[]<>,.:;-+=_|/\\");
}

TEST_CASE("URL Encoding", "[CppCommon][String]")
{
    REQUIRE(Encoding::URLEncode("Sample URL encoding: ~`'\"!?@#$%^&*(){}[]<>,.:;-+=_|/\\") == "Sample+URL+encoding%3A+~%60%27%22%21%3F%40%23%24%25%5E%26%2A%28%29%7B%7D%5B%5D%3C%3E%2C.%3A%3B-%2B%3D_%7C/%5C");
    REQUIRE(Encoding::URLDecode("Sample+URL+encoding%3A+~%60%27%22%21%3F%40%23%24%25%5E%26%2A%28%29%7B%7D%5B%5D%3C%3E%2C.%3A%3B-%2B%3D_%7C/%5C") == "Sample URL encoding: ~`'\"!?@#$%^&*(){}[]<>,.:;-+=_|/\\");
}
