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
        virtual DateTime sunrise() = 0;
        virtual DateTime sunset() = 0;
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
        ArduinoSolar(Clock *clock, Dusk2Dawn *location);
        DateTime sunrise();
        DateTime sunset();

    private:
        Clock *_clock;
        Dusk2Dawn *_location;
    };

    class RTCClock : public Clock
    {
    public:
        DateTime now();

    private:
        RTClib _rtcLib;
    };
#endif

} // namespace rtc

#endif