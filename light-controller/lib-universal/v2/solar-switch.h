#ifndef V2_RTC_HANDLERS
#define V2_RTC_HANDLERS

#include <rtc.h>
#include "pin-bus.h"

namespace v2
{
    class SolarSwitch
    {
    public:
        SolarSwitch(rtc::Solar *solar, PinBus *bus);
        void loop(DateTime now);
        // Positive - after, negative - before
        SolarSwitch *setSunriseOffsetMinutes(const uint8_t offset);
        // Positive - after, negative - before
        SolarSwitch *setSunsetOffsetMinutes(const uint8_t offset);
        // Index of a pin to toggle
        SolarSwitch *setPin(const byte pinIndex, const byte dayState, const byte nightState);

    private:
        SolarSwitch();
        rtc::Solar *_solar;
        PinBus *_bus;
        byte _pinIndex;
        byte _dayState;
        byte _nightState;
    };

} // namespace v2

#endif