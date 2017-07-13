//
// Created by Ivan Shynkarenka on 13.07.2017
//

#include "catch.hpp"

#include "containers/hashmap.h"

using namespace CppCommon;

TEST_CASE("Hash map", "[CppCommon][Containers]")
{
    HashMap<int, int> hashmap(128, -1);
    REQUIRE(hashmap.empty());
    REQUIRE(hashmap.size() == 0);

    hashmap[6] = 6;
    REQUIRE(hashmap.size() == 1);
    hashmap[3] = 3;
    REQUIRE(hashmap.size() == 2);
    hashmap[7] = 7;
    REQUIRE(hashmap.size() == 3);
    hashmap[2] = 2;
    REQUIRE(hashmap.size() == 4);
    hashmap[8] = 8;
    REQUIRE(hashmap.size() == 5);
    hashmap[1] = 1;
    REQUIRE(hashmap.size() == 6);
    hashmap[4] = 4;
    REQUIRE(hashmap.size() == 7);
    hashmap[9] = 9;
    REQUIRE(hashmap.size() == 8);
    hashmap[5] = 5;
    REQUIRE(hashmap.size() == 9);

    REQUIRE(!hashmap.empty());

    int sum = 0;
    for (auto it = hashmap.begin(); it != hashmap.end(); ++it)
    {
        REQUIRE(it->first == it->second);
        sum += it->second;
    }
    REQUIRE(sum == 45);

    sum = 0;
    for (auto it = hashmap.rbegin(); it != hashmap.rend(); ++it)
    {
        REQUIRE(it->first == it->second);
        sum += it->second;
    }
    REQUIRE(sum == 45);

    REQUIRE(!hashmap.find(0));
    REQUIRE(hashmap.find(1));
    REQUIRE(hashmap.find(2));
    REQUIRE(hashmap.find(3));
    REQUIRE(hashmap.find(4));
    REQUIRE(hashmap.find(5));
    REQUIRE(hashmap.find(6));
    REQUIRE(hashmap.find(7));
    REQUIRE(hashmap.find(8));
    REQUIRE(hashmap.find(9));
    REQUIRE(!hashmap.find(10));

    REQUIRE(!hashmap.erase(0));
    REQUIRE(!hashmap.erase(10));

    REQUIRE(hashmap.erase(1));
    REQUIRE(hashmap.size() == 8);
    REQUIRE(hashmap.erase(3));
    REQUIRE(hashmap.size() == 7);
    REQUIRE(hashmap.erase(6));
    REQUIRE(hashmap.size() == 6);
    REQUIRE(hashmap.erase(9));
    REQUIRE(hashmap.size() == 5);

    REQUIRE(!hashmap.find(0));
    REQUIRE(!hashmap.find(1));
    REQUIRE(hashmap.find(2));
    REQUIRE(!hashmap.find(3));
    REQUIRE(hashmap.find(4));
    REQUIRE(hashmap.find(5));
    REQUIRE(!hashmap.find(6));
    REQUIRE(hashmap.find(7));
    REQUIRE(hashmap.find(8));
    REQUIRE(!hashmap.find(9));
    REQUIRE(!hashmap.find(10));

    REQUIRE(hashmap.erase(5));
    REQUIRE(hashmap.size() == 4);
    REQUIRE(hashmap.erase(2));
    REQUIRE(hashmap.size() == 3);
    REQUIRE(hashmap.erase(7));
    REQUIRE(hashmap.size() == 2);
    REQUIRE(hashmap.erase(8));
    REQUIRE(hashmap.size() == 1);
    REQUIRE(hashmap.erase(4));
    REQUIRE(hashmap.size() == 0);

    REQUIRE(hashmap.empty());
}
