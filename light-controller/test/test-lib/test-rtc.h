#ifndef MOCKS_TEST_RTC
#define MOCKS_TEST_RTC

#include <rtc.h>

class TestSolar : public rtc::Solar
{
public:
    TestSolar(DateTime sunrise, DateTime sunset);
    ~TestSolar();
    DateTime sunrise();
    DateTime sunset();

private:
    DateTime _sunrise;
    DateTime _sunset;
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