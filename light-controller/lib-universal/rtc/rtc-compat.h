#ifndef rtc_compat_h
#define rtc_compat_h

#ifdef ARDUINO1

#include <DS3231.h>

#else

#include "stdint.h"

// copy/paste from DS3231.h to be able to run locally
class DateTime
{
public:
    DateTime(uint16_t year, uint8_t month, uint8_t day,
             uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
    uint16_t year() const { return y; }
    uint8_t month() const { return m; }
    uint8_t day() const { return d; }
    uint8_t hour() const { return hh; }
    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }

private:
    uint16_t y;
    uint8_t m, d, hh, mm, ss;
};

#endif

#endif