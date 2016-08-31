/*!
    \file utility_singleton.cpp
    \brief Singleton example
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

#include "utility/singleton.h"

#include <iostream>

class MySingleton : public CppCommon::Singleton<MySingleton>
{
   friend CppCommon::Singleton<MySingleton>;

public:
    void Test() { std::cout << "MySingleton::Test()" << std::endl; }

private:
    MySingleton() { std::cout << "MySingleton::MySingleton()" << std::endl; }
    ~MySingleton() { std::cout << "MySingleton::~MySingleton()" << std::endl; }
};

int main(int argc, char** argv)
{
    MySingleton::GetInstance().Test();
    return 0;
}
