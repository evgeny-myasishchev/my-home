#include "test-rtc.h"

TestClock::TestClock(DateTime *now)
{
    _now = now;
};

DateTime TestClock::now()
{
    return *_now;
}