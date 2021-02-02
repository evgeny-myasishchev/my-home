#include <Dusk2Dawn.h>
#include <arduino-compat.h>
#include <logger.h>
#include <pin-bus.h>
#include <rtc.h>
#include <switch-service-v2.h>
#include <switches-router-v2.h>
#include <solar-switch.h>
#include <Wire.h>
#include <at.h>
#include <at-commands.h>

// Test mode will only use pin bus, see below
// #define TEST_MODE

// #include "routes/test-routes.h"
#include "routes/fl-1-routes.h"
// #include "routes/fl-2-routes.h"
// #include "routes/outdoor-routes.h"

using namespace v2;

PCF8574Bus pcf8574bus(RELAY_BOARDS, INPUT_BOARDS, true);

// Virtual bus is used by software switches (like solar)
InMemoryPinBus virtualBus(1);

const auto busses = new PinBus *[2] { &pcf8574bus, &virtualBus };

CompositePinBus bus(2, busses);

SwitchesRouter *router;

#ifdef AT_ENABLED
io::SerialTextStream atStream(&Serial);
at::Engine atEngine(&atStream);
ArduinoDigitalWrite arduinoDigitalWrite(digitalWrite);
ATPing atPing;
ATLed atLed(LED_BUILTIN, &arduinoDigitalWrite);
ATGetPin atGetPin(&bus);
ATSetPin atSetPin(&bus);
#endif

rtc::RTCClock clock;
Dusk2Dawn location(50.04, 36.30, +2);
rtc::ArduinoSolar solar(&location);
SolarSwitch solarSwitch(&solar, &bus);

ArrayPtr<Switch *> routes = createRoutes(&solarSwitch);

void setup()
{
    Wire.begin();

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    while (!Serial)
    {
    }

#ifdef RTC_SETUP
    {
        rtc::setupClockToCompileTime();
    }
#endif
    logger_setup(&clock, &Serial);

#ifdef RTC_SETUP
    logger_log("RTC clock setup performed.");
#endif

    router = new SwitchesRouter(SwitchRouterServices{
        .bus = &bus,
        .pushBtnSwitchSvc = new PushButtonSwitchService(SwitchServiceConfig()),
        .toggleBtnSwitchSvc = new ToggleButtonSwitchService(SwitchServiceConfig()),
    });

    pcf8574bus.setup(0x00, 0x00);

    auto now = clock.now();
    solarSwitch.setup(now);

#ifdef AT_ENABLED
    atEngine.addCommandHandler(&atPing);
    atEngine.addCommandHandler(&atLed);
    atEngine.addCommandHandler(&atGetPin);
    atEngine.addCommandHandler(&atSetPin);
    atEngine.setup();
#endif

    logger_log("Controller initialized.");
}

void loop()
{
    pcf8574bus.readState();

#ifdef AT_ENABLED
    atEngine.loop();
#endif

#ifndef TEST_MODE

    auto now = clock.now();
    solarSwitch.loop(now);
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

    pcf8574bus.writeState();
}
