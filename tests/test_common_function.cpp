//
// Created by Ivan Shynkarenka on 18.08.2018
//

#include "test.h"

#include "common/function.h"

using namespace CppCommon;

namespace {

int test(int v)
{
    return v + 100;
}

class Class
{
public:
    int operator()(int v) { return v + 200; }
    int test(int v) { return v + 300; }
    static int static_test(int v) { return v + 400; }
};

} // namespace

TEST_CASE("Function", "[CppCommon][Common]")
{
    CppCommon::Function<int (int), 128> function;

    // Simple function call
    function = test;
    REQUIRE(function(11) == 111);

    Class instance;

    // Class operator() call
    function = instance;
    REQUIRE(function(22) == 222);

    // Class method call
    function = std::bind(&Class::test, &instance, std::placeholders::_1);
    REQUIRE(function(33) == 333);

    // Class static method call
    function = Class::static_test;
    REQUIRE(function(44) == 444);

    // Lambda function call
    auto lambda = [=](int v) { return v + 500; };
    function = lambda;
    REQUIRE(function(55) == 555);
}
