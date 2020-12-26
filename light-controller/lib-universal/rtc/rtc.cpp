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

    ArduinoSolar::ArduinoSolar(Dusk2Dawn *location)
    {
        _location = location;
    }

    int ArduinoSolar::sunriseOffsetMinutes(DateTime now)
    {
        // TODO: Support DST
        return _location->sunrise(now.year(), now.month(), now.day(), false);
    }

    int ArduinoSolar::sunsetOffsetMinutes(DateTime now)
    {
        // TODO: Support DST
        return _location->sunset(now.year(), now.month(), now.day(), false);
    }

    DateTime RTCClock::now()
    {
        return _rtcLib.now();
    }
#endif
} // namespace rtc