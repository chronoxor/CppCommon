/*!
    \file interface.h
    \brief Random interface definition
    \author Ivan Shynkarenka
    \date 08.06.2017
    \copyright MIT License
*/

#include "system/dll.h"

#include <memory>

class IRandom
{
public:
    virtual ~IRandom() = default;
    virtual int random() = 0;
};

PLUGIN bool PluginRandomCreate(IRandom** ppRandom);
PLUGIN bool PluginRandomRelease(IRandom* pRandom);
