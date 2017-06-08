/*!
    \file function.cpp
    \brief Random function plugin
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#include "system/dll.h"

#include <cstdlib>

API int PluginRandom()
{
   return std::rand();
}
