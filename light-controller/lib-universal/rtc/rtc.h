#ifndef rtc_h
#define rtc_h

#include "rtc-compat.h"

namespace rtc
{

class Solar {
public:
    virtual DateTime sunrise() = 0;
    virtual DateTime sunset() = 0;
};

class Clock
{
public:
    virtual DateTime now();
};

class Handler
{
public:
    Handler();
    virtual ~Handler();
    virtual void handle(DateTime now) = 0;
};

class Controller {
public:
    Controller(Clock *rtc);
    void addHandler(Handler *handler);
    void setup();
    void loop();
};

}

#endif