#if !defined(SETUP_RTC_H)
#define SETUP_RTC_H

#include <arduino-compat.h>
#include <logger.h>

class SetupRTC
{
private:
    rtc::RTCClock *clock = new rtc::RTCClock();
public:
    void setup();
};

void SetupRTC::setup()
{
    Wire.begin();
    Serial.begin(115200);
    while (!Serial)
    {
    }
    rtc::setupClockToCompileTime();
    logger_setup(clock, &Serial);
    logger_log("RTC clock setup performed.");
}


#endif // SETUP_RTC_H
