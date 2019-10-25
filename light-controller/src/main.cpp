#include <arduino-compat.h>
#include <logger.h>
#include <pin-bus.h>
#include <switch-service-v2.h>
#include <switches-router-v2.h>

// Test mode will only use pin bus, see below
// #define TEST_MODE

#include "routes/test-routes.h"
// #include "routes/fl-2-routes.h"

using namespace v2;

PCF8574Bus bus(RELAY_BOARDS, INPUT_BOARDS, true);

SwitchesRouter *router;

ArrayPtr<Switch *> routes = createRoutes();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    logger_setup(&Serial);

    router = new SwitchesRouter(SwitchRouterServices{
        .bus = &bus,
        .pushBtnSwitchSvc = new PushButtonSwitchService(SwitchServiceConfig()),
        .toggleBtnSwitchSvc = new ToggleButtonSwitchService(SwitchServiceConfig()),
    });

    bus.setup(0xFF);

    logger_log("Controller initialized.");
}

void loop()
{
    bus.readState();

    #ifndef TEST_MODE

    router->processRoutes(routes);

    #else

    for (size_t relayIndex = 0; relayIndex < RELAY_BOARDS; relayIndex++)
    {
        for (size_t bit = 0; bit < 8; bit++)
        {
            const byte relayAddress = relayIndex * 8 + bit;
            const byte relaySwitchAddress = RELAY_BOARDS * 8 + relayAddress;
            const auto relaySwitchState = bus.getPin(relaySwitchAddress);
            bus.setPin(relayIndex * 8 + bit, relaySwitchState);
        }
    }

    const byte switchAllState = bus.getPin((RELAY_BOARDS + INPUT_BOARDS - 1) * 8);
    if(switchAllState == LOW)
    {
        for (size_t i = 0; i < RELAY_BOARDS * 8; i++)
        {
            bus.setPin(i, LOW);
        }
    }

    #endif

    bus.writeState();
}
