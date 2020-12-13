#include "test-rtc.h"

TestSolar::TestSolar(DateTime sunrise, DateTime sunset)
{
    _sunrise = sunrise;
    _sunset = sunset;
};

TestSolar::~TestSolar()
{
}

DateTime TestSolar::sunrise()
{
    return _sunrise;
}

DateTime TestSolar::sunset()
{
    return _sunset;
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