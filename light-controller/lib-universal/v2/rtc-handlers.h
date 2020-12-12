#ifndef V2_RTC_HANDLERS
#define V2_RTC_HANDLERS

#include <rtc.h>
#include "pin-bus.h"

namespace v2
{

    class SolarHandler : rtc::Handler
    {
    public:
        SolarHandler(rtc::Solar *solar, PinBus *bus);
        ~SolarHandler();
        void handle(DateTime now);
        // Positive - after, negative - before
        SolarHandler* setSunriseOffsetMinutes(const uint8_t offset);
        // Positive - after, negative - before
        SolarHandler* setSunsetOffsetMinutes(const uint8_t offset);
        // Index of a pin to toggle
        SolarHandler* setPin(const byte pinIndex, const byte dayState, const byte nightState);
    };

} // namespace v2

#endif