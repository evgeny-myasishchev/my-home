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
    };

    TEST_F(SolarSwitchLoop, SetDayState)
    {
        v2::SolarSwitch sw(&solar, &bus);
        sw.setPin(9, 1, 0);

        sw.loop(DateTime(2020, 12, 13, 10, 27, 55));
        ASSERT_EQ(1, bus.getPin(9)) << "minute after sunrise";

        sw.loop(DateTime(2020, 12, 13, 11, 34, 55));
        ASSERT_EQ(1, bus.getPin(9)) << "day hour but minute after sunset";

        sw.loop(DateTime(2020, 12, 13, 12, 25, 55));
        ASSERT_EQ(1, bus.getPin(9)) << "day hour but minute before sunraise";

        sw.loop(DateTime(2020, 12, 13, 15, 27, 55));
        ASSERT_EQ(1, bus.getPin(9)) << "sunset day hour but minute after sunraise";;
    }

    TEST_F(SolarSwitchLoop, SetDayStateWithOffsetsLessThanHour)
    {
        v2::SolarSwitch sw(&solar, &bus);
        sw.setSunriseOffsetMinutes(4);
        sw.setPin(9, 1, 0);

        sw.loop(DateTime(2020, 12, 13, 07, 29, 55));
        ASSERT_EQ(0, bus.getPin(9));

        sw.loop(DateTime(2020, 12, 13, 07, 30, 55));
        ASSERT_EQ(1, bus.getPin(9));
    }

    TEST_F(SolarSwitchLoop, SetDayStateWithOffsetsMoreThanHour)
    {
        v2::SolarSwitch sw(&solar, &bus);
        sw.setSunriseOffsetMinutes(124);
        sw.setPin(9, 1, 0);

        sw.loop(DateTime(2020, 12, 13, 9, 29, 55));
        ASSERT_EQ(0, bus.getPin(9));

        sw.loop(DateTime(2020, 12, 13, 9, 30, 55));
        ASSERT_EQ(1, bus.getPin(9));
    }

    TEST_F(SolarSwitchLoop, SetNightState)
    {
        v2::SolarSwitch sw(&solar, &bus);
        sw.setPin(9, 0, 1);
        sw.loop(DateTime(2020, 12, 13, 16, 30, 55));

        ASSERT_EQ(1, bus.getPin(9));
    }

    TEST_F(SolarSwitchLoop, SetNightStateWithOffsetsLessThanHour)
    {
        v2::SolarSwitch sw(&solar, &bus);
        sw.setPin(9, 0, 1);
        sw.setSunsetOffsetMinutes(4);

        sw.loop(DateTime(2020, 12, 13, 15, 37, 55));
        ASSERT_EQ(0, bus.getPin(9));

        sw.loop(DateTime(2020, 12, 13, 15, 38, 55));
        ASSERT_EQ(1, bus.getPin(9));
    }

    TEST_F(SolarSwitchLoop, SetNightStateWithOffsetsMoreThanHour)
    {
        v2::SolarSwitch sw(&solar, &bus);
        sw.setPin(9, 0, 1);
        sw.setSunsetOffsetMinutes(124);

        sw.loop(DateTime(2020, 12, 13, 17, 37, 55));
        ASSERT_EQ(0, bus.getPin(9));

        sw.loop(DateTime(2020, 12, 13, 17, 38, 55));
        ASSERT_EQ(1, bus.getPin(9));
    }

} // namespace
