#include "test-rtc.h"

TestSolar::TestSolar(int sunriseOffsetMins, int sunsetOffsetMins)
{
    _sunriseOffsetMins = sunriseOffsetMins;
    _sunsetOffsetMins = sunsetOffsetMins;
};

TestSolar::~TestSolar()
{
}

int TestSolar::sunriseOffsetMinutes(DateTime now)
{
    return _sunriseOffsetMins;
}

int TestSolar::sunsetOffsetMinutes(DateTime now)
{
    return _sunsetOffsetMins;
}

TestClock::TestClock(DateTime *now)
{
    _now = now;
};

TestClock::~TestClock()
{
};

DateTime TestClock::now()
{
    return *_now;
}