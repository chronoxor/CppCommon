/*!
    \file common_function.cpp
    \brief Allocation free function example
    \author Ivan Shynkarenka
    \date 18.08.2017
    \copyright MIT License
*/

#include "common/function.h"

#include <iostream>

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

int main(int argc, char** argv)
{
    CppCommon::Function<int (int), 128> function;

    // Simple function call
    function = test;
    std::cout << "test(11) = " << function(11) << std::endl;

    Class instance;

    // Class operator() call
    function = instance;
    std::cout << "Class::operator(22) = " << function(22) << std::endl;

    // Class method call
    function = std::bind(&Class::test, &instance, std::placeholders::_1);
    std::cout << "Class::test(33) = " << function(33) << std::endl;

    // Class static method call
    function = Class::static_test;
    std::cout << "Class::static_test(44) = " << function(44) << std::endl;

    // Lambda function call
    auto lambda = [=](int v) { return v + 500; };
    function = lambda;
    std::cout << "lambda(55) = " << function(55) << std::endl;

    return 0;
}
