#ifndef rtc_h
#define rtc_h

#include "rtc-compat.h"

#ifdef ARDUINO
#include <Dusk2Dawn.h>
#include <DS3231.h>
#endif

namespace rtc
{
    class Solar
    {
    public:
        virtual int sunriseOffsetMinutes(DateTime now) = 0;
        virtual int sunsetOffsetMinutes(DateTime now) = 0;
    };

    class Clock
    {
    public:
        virtual DateTime now() = 0;
    };

#ifdef ARDUINO
    class ArduinoSolar : public Solar
    {
    public:
        ArduinoSolar(Dusk2Dawn *location);
        int sunriseOffsetMinutes(DateTime now);
        int sunsetOffsetMinutes(DateTime now);

    private:
        Dusk2Dawn *_location;
    };

    class RTCClock : public Clock
    {
    public:
        DateTime now();

    private:
        RTClib _rtcLib;
    };
#else
    class UniversalClock : public Clock
    {
    public:
        DateTime now();
    };
#endif

} // namespace rtc

#endif