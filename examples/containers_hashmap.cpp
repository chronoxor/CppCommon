/*!
    \file containers_hashmap.cpp
    \brief Hash map container example
    \author Ivan Shynkarenka
    \date 13.07.2017
    \copyright MIT License
*/

#include "containers/hashmap.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    CppCommon::HashMap<std::string, int> hashmap;

    hashmap["item6"] = 6;
    hashmap["item3"] = 3;
    hashmap["item7"] = 7;
    hashmap["item2"] = 2;
    hashmap["item8"] = 8;
    hashmap["item1"] = 1;
    hashmap["item4"] = 4;
    hashmap["item9"] = 9;
    hashmap["item5"] = 5;

    std::cout << "hashmap:" << std::endl;
    for (const auto& item : hashmap)
        std::cout << item.first << " => " << item.second << std::endl;

    return 0;
}
