#include "rtc-solar.h"

namespace rtc
{
#ifdef ARDUINO

#ifndef RTC_DATE
#define RTC_DATE __DATE__
#endif

#ifndef RTC_TIME
#define RTC_TIME __TIME__
#endif
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
#else
    #include <time.h>
    DateTime UniversalClock::now()
    {
        time_t t = time(NULL);
        const auto loctime = localtime(&t);
        return DateTime(
            loctime->tm_year, loctime->tm_mon, loctime->tm_mday,
            loctime->tm_hour, loctime->tm_min, loctime->tm_sec
        );
    }
#endif
} // namespace rtc