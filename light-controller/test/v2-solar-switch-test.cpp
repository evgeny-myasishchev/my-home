#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "solar-switch.h"
#include "test-lib/test-pin-bus.h"
#include "test-lib/test-rtc.h"

namespace
{

    class SolarSwitchLoop : public ::testing::Test
    {
    protected:
        TestPinBus bus;
        DateTime sunrise;
        DateTime sunset;
        TestSolar solar;

        SolarSwitchLoop() : 
            bus(2), 
            sunrise(2020, 12, 13, 07, 26, 0), 
            sunset(2020, 12, 13, 15, 33, 0),
            solar(sunrise, sunset)
        {

        }

        void SetUp() override
        {
        }
    };

    TEST_F(SolarSwitchLoop, SetDayState)
    {
        DateTime now(2020, 12, 13, 10, 30, 55);
        
        v2::SolarSwitch sw(&solar, &bus);
        sw.setPin(9, 1, 0);
        sw.loop(now);

        ASSERT_EQ(1, bus.getPin(9));
    }

    TEST_F(SolarSwitchLoop, SetNightState)
    {
        DateTime now(2020, 12, 13, 16, 30, 55);
        
        v2::SolarSwitch sw(&solar, &bus);
        sw.setPin(9, 0, 1);
        sw.loop(now);

        ASSERT_EQ(1, bus.getPin(9));
    }

} // namespace
