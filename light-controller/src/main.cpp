
#include "app.h"
#include "routes/test-routes.h"
// #include "routes/fl-1-routes.h"
// #include "routes/fl-2-routes.h"
// #include "routes/outdoor-routes.h"

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

long lastPingAgo;
void loop()
{
    const auto now = millis();
    if(now - lastPingAgo >= 1000) {
        lastPingAgo = now;
        logger_log("PING: %d", lastPingAgo);
    }
    app.loop();
}
