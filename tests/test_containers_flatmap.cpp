//
// Created by Ivan Shynkarenka on 22.06.2017
//

#include "test.h"

#include "containers/flatmap.h"

using namespace CppCommon;

TEST_CASE("Flat map", "[CppCommon][Containers]")
{
    FlatMap<int, int> flatmap;
    REQUIRE(flatmap.empty());
    REQUIRE(flatmap.size() == 0);

    flatmap[6] = 6;
    REQUIRE(flatmap.size() == 1);
    flatmap[3] = 3;
    REQUIRE(flatmap.size() == 2);
    flatmap[7] = 7;
    REQUIRE(flatmap.size() == 3);
    flatmap[2] = 2;
    REQUIRE(flatmap.size() == 4);
    flatmap[8] = 8;
    REQUIRE(flatmap.size() == 5);
    flatmap[1] = 1;
    REQUIRE(flatmap.size() == 6);
    flatmap[4] = 4;
    REQUIRE(flatmap.size() == 7);
    flatmap[9] = 9;
    REQUIRE(flatmap.size() == 8);
    flatmap[5] = 5;
    REQUIRE(flatmap.size() == 9);

    REQUIRE(!flatmap.empty());

    int sum = 0;
    int prev = 0;
    for (auto it = flatmap.begin(); it != flatmap.end(); ++it)
    {
        REQUIRE(prev < it->second);
        prev = it->second;
        sum += it->second;
    }
    REQUIRE(sum == 45);

    sum = 0;
    prev = 10;
    for (auto it = flatmap.rbegin(); it != flatmap.rend(); ++it)
    {
        REQUIRE(prev > it->second);
        prev = it->second;
        sum += it->second;
    }
    REQUIRE(sum == 45);

    REQUIRE(flatmap.find(0) == flatmap.end());
    REQUIRE(flatmap.find(1) != flatmap.end());
    REQUIRE(flatmap.find(2) != flatmap.end());
    REQUIRE(flatmap.find(3) != flatmap.end());
    REQUIRE(flatmap.find(4) != flatmap.end());
    REQUIRE(flatmap.find(5) != flatmap.end());
    REQUIRE(flatmap.find(6) != flatmap.end());
    REQUIRE(flatmap.find(7) != flatmap.end());
    REQUIRE(flatmap.find(8) != flatmap.end());
    REQUIRE(flatmap.find(9) != flatmap.end());
    REQUIRE(flatmap.find(10) == flatmap.end());

    REQUIRE(flatmap.lower_bound(0)->second == 1);
    REQUIRE(flatmap.lower_bound(1)->second == 1);
    REQUIRE(flatmap.lower_bound(2)->second == 2);
    REQUIRE(flatmap.lower_bound(3)->second == 3);
    REQUIRE(flatmap.lower_bound(4)->second == 4);
    REQUIRE(flatmap.lower_bound(5)->second == 5);
    REQUIRE(flatmap.lower_bound(6)->second == 6);
    REQUIRE(flatmap.lower_bound(7)->second == 7);
    REQUIRE(flatmap.lower_bound(8)->second == 8);
    REQUIRE(flatmap.lower_bound(9)->second == 9);
    REQUIRE(flatmap.lower_bound(10) == flatmap.end());

    REQUIRE(flatmap.upper_bound(0)->second == 1);
    REQUIRE(flatmap.upper_bound(1)->second == 2);
    REQUIRE(flatmap.upper_bound(2)->second == 3);
    REQUIRE(flatmap.upper_bound(3)->second == 4);
    REQUIRE(flatmap.upper_bound(4)->second == 5);
    REQUIRE(flatmap.upper_bound(5)->second == 6);
    REQUIRE(flatmap.upper_bound(6)->second == 7);
    REQUIRE(flatmap.upper_bound(7)->second == 8);
    REQUIRE(flatmap.upper_bound(8)->second == 9);
    REQUIRE(flatmap.upper_bound(9) == flatmap.end());

    REQUIRE(flatmap.erase(0) == 0);
    REQUIRE(flatmap.erase(10) == 0);

    REQUIRE(flatmap.erase(1) == 1);
    REQUIRE(flatmap.size() == 8);
    REQUIRE(flatmap.erase(3) == 1);
    REQUIRE(flatmap.size() == 7);
    REQUIRE(flatmap.erase(6) == 1);
    REQUIRE(flatmap.size() == 6);
    REQUIRE(flatmap.erase(9) == 1);
    REQUIRE(flatmap.size() == 5);

    REQUIRE(flatmap.find(0) == flatmap.end());
    REQUIRE(flatmap.find(1) == flatmap.end());
    REQUIRE(flatmap.find(2) != flatmap.end());
    REQUIRE(flatmap.find(3) == flatmap.end());
    REQUIRE(flatmap.find(4) != flatmap.end());
    REQUIRE(flatmap.find(5) != flatmap.end());
    REQUIRE(flatmap.find(6) == flatmap.end());
    REQUIRE(flatmap.find(7) != flatmap.end());
    REQUIRE(flatmap.find(8) != flatmap.end());
    REQUIRE(flatmap.find(9) == flatmap.end());
    REQUIRE(flatmap.find(10) == flatmap.end());

    REQUIRE(flatmap.lower_bound(0)->second == 2);
    REQUIRE(flatmap.lower_bound(1)->second == 2);
    REQUIRE(flatmap.lower_bound(2)->second == 2);
    REQUIRE(flatmap.lower_bound(3)->second == 4);
    REQUIRE(flatmap.lower_bound(4)->second == 4);
    REQUIRE(flatmap.lower_bound(5)->second == 5);
    REQUIRE(flatmap.lower_bound(6)->second == 7);
    REQUIRE(flatmap.lower_bound(7)->second == 7);
    REQUIRE(flatmap.lower_bound(8)->second == 8);
    REQUIRE(flatmap.lower_bound(9) == flatmap.end());
    REQUIRE(flatmap.lower_bound(10) == flatmap.end());

    REQUIRE(flatmap.upper_bound(0)->second == 2);
    REQUIRE(flatmap.upper_bound(1)->second == 2);
    REQUIRE(flatmap.upper_bound(2)->second == 4);
    REQUIRE(flatmap.upper_bound(3)->second == 4);
    REQUIRE(flatmap.upper_bound(4)->second == 5);
    REQUIRE(flatmap.upper_bound(5)->second == 7);
    REQUIRE(flatmap.upper_bound(6)->second == 7);
    REQUIRE(flatmap.upper_bound(7)->second == 8);
    REQUIRE(flatmap.upper_bound(8) == flatmap.end());
    REQUIRE(flatmap.upper_bound(9) == flatmap.end());

    REQUIRE(flatmap.erase(5) == 1);
    REQUIRE(flatmap.size() == 4);
    REQUIRE(flatmap.erase(2) == 1);
    REQUIRE(flatmap.size() == 3);
    REQUIRE(flatmap.erase(7) == 1);
    REQUIRE(flatmap.size() == 2);
    REQUIRE(flatmap.erase(8) == 1);
    REQUIRE(flatmap.size() == 1);
    REQUIRE(flatmap.erase(4) == 1);
    REQUIRE(flatmap.size() == 0);

    REQUIRE(flatmap.empty());
}
