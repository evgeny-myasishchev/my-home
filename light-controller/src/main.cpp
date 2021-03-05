
#include "app.h"
#include "routes/test-routes.h"
// #include "routes/fl-1-routes.h"
// #include "routes/fl-2-routes.h"
// #include "routes/outdoor-routes.h"

#ifdef PING
#include "ping.h"
Ping ping;
#endif

App app(RELAY_BOARDS, INPUT_BOARDS, createRoutes);

void setup()
{
    app.setup();
    
// #ifdef RTC_SETUP
//     {
//         rtc::setupClockToCompileTime();
//     }
// #endif

// #ifdef RTC_SETUP
//     logger_log("RTC clock setup performed.");
// #endif

}

void loop()
{
#ifdef PING
    ping.loop();
#endif
    app.loop();
}
