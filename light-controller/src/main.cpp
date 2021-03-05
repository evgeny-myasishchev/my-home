
#ifdef PING
#include "ping.h"
Ping ping;
#endif

#ifdef RTC_SETUP
#include "setup-rtc.h"
SetupRTC setupRTC;
#else
#include "app.h"
#include "routes/test-routes.h"
// #include "routes/fl-1-routes.h"
// #include "routes/fl-2-routes.h"
// #include "routes/outdoor-routes.h"
App app(RELAY_BOARDS, INPUT_BOARDS, createRoutes);
#endif

void setup()
{
#ifdef RTC_SETUP
    setupRTC.setup();
#else
    app.setup();
#endif
}

void loop()
{
#ifdef PING
    ping.loop();
#endif

#ifndef RTC_SETUP
    app.loop();
#endif
}
