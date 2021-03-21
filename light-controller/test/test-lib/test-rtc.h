#ifndef MOCKS_TEST_RTC
#define MOCKS_TEST_RTC

#include <rtc-solar.h>

class TestSolar : public rtc::Solar
{
public:
    TestSolar(int sunriseOffsetMins, int sunsetOffsetMins);
    ~TestSolar();
    int sunriseOffsetMinutes(DateTime now);
    int sunsetOffsetMinutes(DateTime now);

private:
    int _sunriseOffsetMins;
    int _sunsetOffsetMins;
};

class TestClock : public rtc::Clock
{
public:
    TestClock(DateTime *now);
    ~TestClock();
    DateTime now();

private:
    DateTime *_now;
};

#endif