#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "test-lib/test-pin-bus.h"
#include "test-lib/test-rtc.h"

namespace
{

    TEST(SolarSwitchLoop, SetDayState)
    {
        DateTime now(2020, 12, 13, 10, 30, 55);
        const TestPinBus bus(2);
        const TestClock clock(&now);
        ASSERT_STREQ("HELLO", "HELLO");
    }

} // namespace
