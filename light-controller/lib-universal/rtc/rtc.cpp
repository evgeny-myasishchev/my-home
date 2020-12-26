#include "rtc.h"

namespace rtc
{
#ifdef ARDUINO

#ifndef RTC_DATE
#define RTC_DATE __DATE__
#endif

#ifndef RTC_TIME
#define RTC_TIME __TIME__
#endif

    void setupClockToCompileTime()
    {
        const auto now = DateTime(RTC_DATE, RTC_TIME);

        logger_log("Setup clock to: %d:%d:%d %d:%d:%d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

        DS3231 clock;
        clock.enableOscillator(false, false, 3);
        clock.setClockMode(false);
        clock.setYear(now.year() - 2000); // last two digits only starting from 2k
        clock.setMonth(now.month());
        clock.setDate(now.day());
        clock.setHour(now.hour());
        clock.setMinute(now.minute());
        clock.setSecond(now.second());
        clock.enableOscillator(true, true, 3);
    }

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