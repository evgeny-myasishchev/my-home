#ifndef V2_SOLAR_SWITCH
#define V2_SOLAR_SWITCH

#include <rtc-solar.h>
#include <logger.h>
#include "pin-bus.h"

namespace v2
{

#ifdef SOLAR_SWITCH_VERBOSE
#define solar_switch_log logger_log
#else
#define solar_switch_log
#endif

    class SolarSwitch
    {
    public:
        SolarSwitch(rtc::Solar *solar, PinBus *bus);
        void setup(DateTime now);
        void loop(DateTime now);
        // Positive - after, negative - before
        void setSunriseOffsetMinutes(const uint8_t offset);
        // Positive - after, negative - before
        void setSunsetOffsetMinutes(const uint8_t offset);
        // Index of a pin to toggle
        void setPin(const byte pinIndex, const byte dayState, const byte nightState);

    private:
        SolarSwitch();
        rtc::Solar *_solar;
        PinBus *_bus;
        byte _pinIndex;
        byte _dayState;
        byte _nightState;

        uint8_t _sunrizeOffsetMinutes = 0;
        uint8_t _sunsetOffsetMinutes = 0;
    };

} // namespace v2

#endif