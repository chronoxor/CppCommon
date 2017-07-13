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

    hashmap.emplace("item6", 6);
    hashmap.emplace("item3", 3);
    hashmap.emplace("item7", 7);
    hashmap.emplace("item2", 2);
    hashmap.emplace("item8", 8);
    hashmap.emplace("item1", 1);
    hashmap.emplace("item4", 4);
    hashmap.emplace("item9", 9);
    hashmap.emplace("item5", 5);

    std::cout << "hashmap:" << std::endl;
    for (auto& item : hashmap)
        std::cout << item.first << " => " << item.second << std::endl;

    return 0;
}
