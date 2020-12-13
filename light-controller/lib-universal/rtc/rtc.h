#ifndef rtc_h
#define rtc_h

#include "rtc-compat.h"

namespace rtc
{

    class Solar
    {
    public:
        virtual DateTime sunrise() = 0;
        virtual DateTime sunset() = 0;
    };

    class Clock
    {
    public:
        virtual DateTime now() = 0;
    };

} // namespace rtc

#endif