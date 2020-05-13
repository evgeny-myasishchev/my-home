#include <arduino-compat.h>
#include <logger.h>
#include <pin-bus.h>
#include <switch-service-v2.h>
#include <switches-router-v2.h>
#include <at.h>
#include <at-commands.h>

// Test mode will only use pin bus, see below
// #define TEST_MODE 

// #include "routes/test-routes.h"
#include "routes/fl-2-routes.h"
// #include "routes/fl-1-routes.h"

using namespace v2;

PCF8574Bus bus(RELAY_BOARDS, INPUT_BOARDS, true);

SwitchesRouter *router;

ArrayPtr<Switch *> routes = createRoutes();

io::SerialTextStream atStream(&Serial);
at::Engine atEngine(&atStream);

ArduinoDigitalWrite arduinoDigitalWrite(digitalWrite);
ATPing atPing;
ATLed atLed(LED_BUILTIN, &arduinoDigitalWrite);
ATGetPin atGetPin(&bus);
ATSetPin atSetPin(&bus);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
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

    bus.setup(0x00, 0x00);

    atEngine.addCommandHandler(&atPing);
    atEngine.addCommandHandler(&atLed);
    atEngine.addCommandHandler(&atGetPin);
    atEngine.addCommandHandler(&atSetPin);
    atEngine.setup();

    logger_log("Controller initialized.");
}

void loop()
{
    bus.readState();

    atEngine.loop();

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
            bus.setPin(relayAddress, relaySwitchState);
        }
    }

    const byte switchAllState = bus.getPin((RELAY_BOARDS + INPUT_BOARDS - 1) * 8);
    if (switchAllState == HIGH)
    {
        for (size_t i = 0; i < RELAY_BOARDS * 8; i++)
        {
            bus.setPin(i, HIGH);
        }
    }

#endif

    bus.writeState();
}
