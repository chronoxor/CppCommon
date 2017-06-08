/*!
    \file interface.cpp
    \brief Random interface implementation
    \author Ivan Shynkarenka
    \date 08.06.2017
    \copyright MIT License
*/

#include "interface.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

class Random : public IRandom
{
public:
    Random();
    virtual ~Random();
    int random() override;
};

Random::Random()
{
    std::cout << "Random()" << std::endl;
    std::srand((unsigned)std::time(nullptr));
}

Random::~Random()
{
    std::cout << "~Random()" << std::endl;
}

int Random::random()
{
    return std::rand();
}

bool PluginRandomCreate(IRandom** ppRandom)
{
    if (ppRandom == nullptr)
        return false;

    *ppRandom = new Random();

    return true;
}

bool PluginRandomRelease(IRandom* pRandom)
{
    if (pRandom == nullptr)
        return false;

    delete pRandom;

    return true;
}
