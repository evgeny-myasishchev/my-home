#ifndef rtc_h
#define rtc_h

#include "rtc-compat.h"

namespace timers
{

class TimeProvider
{
public:
    virtual DateTime now();
};

class RTCHandler
{
public:
    RTCHandler();
    virtual ~RTCHandler();
    virtual void Handle(DateTime now) = 0;
};

class RTCController {
public:
    RTCController(TimeProvider *timeProvider);
    void addHandler(RTCHandler *handler);
    void setup();
    void loop();
};

}

#endif