//
// Created by Ivan Shynkarenka on 31.08.2016
//

#include "test.h"

#include "utility/singleton.h"

using namespace CppCommon;

namespace {

class TestSingleton : public Singleton<TestSingleton>
{
   friend Singleton<TestSingleton>;

public:
    bool Test() { return _init; }

private:
    bool _init;
    TestSingleton() : _init(true) {}
    ~TestSingleton() { _init = false; }
};

} // namespace

TEST_CASE("Singleton", "[CppCommon][Utility]")
{
    REQUIRE(TestSingleton::GetInstance().Test());
}
