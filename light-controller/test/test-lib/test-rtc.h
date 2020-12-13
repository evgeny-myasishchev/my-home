#ifndef MOCKS_TEST_RTC
#define MOCKS_TEST_RTC

#include <rtc.h>

class TestSolar : public rtc::Solar
{
public:
    DateTime sunrise(DateTime now);
    DateTime sunset(DateTime now);
};

class TestClock : rtc::Clock
{
public:
    TestClock(DateTime *now);
    DateTime now();

private:
    DateTime *_now;
};

#endif