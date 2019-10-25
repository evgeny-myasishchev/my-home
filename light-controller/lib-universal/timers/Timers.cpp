#include <Timers.h>

#ifndef UNIVERSAL

#include <Arduino.h>
unsigned long Timers::millis() {
    return ::millis();
}

#else

#include <sys/time.h>
unsigned long Timers::millis() {
    return time(NULL);
}

#endif
