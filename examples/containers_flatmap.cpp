/*!
    \file containers_flatmap.cpp
    \brief Flat map container example
    \author Ivan Shynkarenka
    \date 27.07.2017
    \copyright MIT License
*/

#include "containers/flatmap.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    CppCommon::FlatMap<std::string, int> flatmap;

    flatmap["item6"] = 6;
    flatmap["item3"] = 3;
    flatmap["item7"] = 7;
    flatmap["item2"] = 2;
    flatmap["item8"] = 8;
    flatmap["item1"] = 1;
    flatmap["item4"] = 4;
    flatmap["item9"] = 9;
    flatmap["item5"] = 5;

    std::cout << "flatmap:" << std::endl;
    for (const auto& item : flatmap)
        std::cout << item.first << " => " << item.second << std::endl;

    return 0;
}
