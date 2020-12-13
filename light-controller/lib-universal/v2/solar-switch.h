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