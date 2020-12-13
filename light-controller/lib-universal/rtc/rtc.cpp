#include "rtc.h"

namespace rtc
{
#ifdef ARDUINO
    ArduinoSolar::ArduinoSolar(Clock *clock, Dusk2Dawn *location)
    {
        _clock = clock;
        _location = location;
    }

    DateTime ArduinoSolar::sunrise()
    {
        auto now = _clock->now();
        auto offset = _location->sunrise(now.year(), now.month(), now.day(), false);
        auto offsetHours = offset / 60;
        auto offsetMinutes = offset % 60;
        return DateTime(now.year(), now.month(), now.day(), offsetHours, offsetMinutes, 0);
    }

    DateTime ArduinoSolar::sunset()
    {
        auto now = _clock->now();
        auto offset = _location->sunset(now.year(), now.month(), now.day(), false);
        auto offsetHours = offset / 60;
        auto offsetMinutes = offset % 60;
        return DateTime(now.year(), now.month(), now.day(), offsetHours, offsetMinutes, 0);
    }

    DateTime RTCClock::now()
    {
        return _rtcLib.now();
    }
#endif
} // namespace rtc