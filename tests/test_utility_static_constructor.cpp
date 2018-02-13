//
// Created by Ivan Shynkarenka on 31.08.2016
//

#include "test.h"

#include "utility/static_constructor.h"

using namespace CppCommon;

namespace {

class TestConstructor
{
public:
    TestConstructor() { REQUIRE(value == 123); }
    ~TestConstructor() { REQUIRE(value == 123); }

    int Test() { return value; }

private:
    static int value;

    static void StaticConstructor()
    {
        CppCommon::StaticConstructor<&TestConstructor::StaticConstructor>::instance();
        value = 123;
    }
};

int TestConstructor::value = 0;

class TestDestructor
{
public:
    TestDestructor() { REQUIRE(value == 321); }
    ~TestDestructor() { REQUIRE(value == 321); }

    int Test() { return value; }

private:
    static int value;

    static void StaticConstructor()
    {
        CppCommon::StaticConstructor<&TestDestructor::StaticConstructor, &TestDestructor::StaticDestructor>::instance();
        value = 321;
    }

    static void StaticDestructor()
    {
        value = 0;
    }
};

int TestDestructor::value = 0;

} // namespace

TEST_CASE("Static constructor", "[CppCommon][Utility]")
{
    TestConstructor test;
    REQUIRE(test.Test() == 123);
}

TEST_CASE("Static constructor and destructor", "[CppCommon][Utility]")
{
    TestDestructor test;
    REQUIRE(test.Test() == 321);
}
