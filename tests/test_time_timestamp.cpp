//
// Created by Ivan Shynkarenka on 26.01.2016
//

#include "test.h"

#include "time/timestamp.h"

#include <thread>

using namespace CppCommon;

TEST_CASE("Timestamp", "[CppCommon][Time]")
{
    REQUIRE((Timestamp::epoch() == 0));
    REQUIRE((Timestamp::utc() > 0));
    REQUIRE((Timestamp::local() > 0));
    REQUIRE((Timestamp::nano() > 0));
    REQUIRE((Timestamp::rdts() > 0));

    uint64_t prev_utc = 0;
    uint64_t prev_local = 0;
    uint64_t prev_nano = 0;
    uint64_t prev_rdts = 0;
    for (int i = 0; i < 1000; ++i)
    {
        uint64_t next_utc = Timestamp::utc();
        uint64_t next_local = Timestamp::local();
        uint64_t next_nano = Timestamp::nano();
        uint64_t next_rdts = Timestamp::rdts();
        REQUIRE(prev_utc <= next_utc);
        REQUIRE(prev_local <= next_local);
        REQUIRE(prev_nano <= next_nano);
        REQUIRE(prev_rdts <= next_rdts);
        prev_utc = next_utc;
        prev_local = next_local;
        prev_nano = next_nano;
        prev_rdts = next_rdts;
    }

    // Compatibility with std::chrono
    Timestamp timestamp(std::chrono::system_clock::now() + std::chrono::milliseconds(10));
    std::this_thread::sleep_until(timestamp.chrono());
}
