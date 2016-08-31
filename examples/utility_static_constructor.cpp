/*!
    \file utility_static_constructor.cpp
    \brief Static constructor pattern example
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

#include "utility/static_constructor.h"

#include <iostream>

class MyClass
{
public:
    MyClass() { std::cout << "MyClass::MyClass() - MyClass::value = " << value << std::endl; }
    ~MyClass() { std::cout << "MyClass::~MyClass() - MyClass::value = " << value << std::endl; }

    void Test() { std::cout << "MyClass::Test() - MyClass::value = " << value << std::endl; }

private:
    static int value;

    static void StaticConstructor()
    {
        CppCommon::StaticConstructor<&MyClass::StaticConstructor, &MyClass::StaticDestructor>::instance();

        value = 123;
        std::cout << "MyClass::StaticConstructor() - MyClass::value = " << value << std::endl;
    }

    static void StaticDestructor()
    {
        value = 0;
        std::cout << "MyClass::StaticDestructor() - MyClass::value = " << value << std::endl;
    }
};

int MyClass::value = 0;

int main(int argc, char** argv)
{
    MyClass instance;
    instance.Test();
    return 0;
}
