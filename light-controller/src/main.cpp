#include <Dusk2Dawn.h>
#include <arduino-compat.h>
#include <logger.h>
#include <pin-bus.h>
#include <rtc.h>
#include <switch-service-v2.h>
#include <switches-router-v2.h>
#include <solar-switch.h>
#include <at.h>
#include <at-commands.h>
#include <Wire.h>


// Test mode will only use pin bus, see below
// #define TEST_MODE 

// #include "routes/test-routes.h"
// #include "routes/fl-1-routes.h"
// #include "routes/fl-2-routes.h"
#include "routes/outdoor-routes.h"

using namespace v2;

PCF8574Bus pcf8574bus(RELAY_BOARDS, INPUT_BOARDS, true);

// Virtual bus is used by software switches (like solar)
InMemoryPinBus virtualBus(1);

const auto busses = new PinBus*[2]{&pcf8574bus, &virtualBus};

CompositePinBus bus(2, busses);

SwitchesRouter *router;


io::SerialTextStream atStream(&Serial);
at::Engine atEngine(&atStream);

ArduinoDigitalWrite arduinoDigitalWrite(digitalWrite);
ATPing atPing;
ATLed atLed(LED_BUILTIN, &arduinoDigitalWrite);
ATGetPin atGetPin(&bus);
ATSetPin atSetPin(&bus);

rtc::RTCClock clock;
Dusk2Dawn location(50.04, 36.30, +2);
rtc::ArduinoSolar solar(&clock, &location);
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
    logger_setup(&Serial);

#ifdef RTC_SETUP
    rtc::setupClockToCompileTime();
#endif

    router = new SwitchesRouter(SwitchRouterServices{
        .bus = &pcf8574bus,
        .pushBtnSwitchSvc = new PushButtonSwitchService(SwitchServiceConfig()),
        .toggleBtnSwitchSvc = new ToggleButtonSwitchService(SwitchServiceConfig()),
    });

    pcf8574bus.setup(0x00, 0x00);

    solarSwitch.setup();

    atEngine.addCommandHandler(&atPing);
    atEngine.addCommandHandler(&atLed);
    atEngine.addCommandHandler(&atGetPin);
    atEngine.addCommandHandler(&atSetPin);
    atEngine.setup();

    logger_log("Controller initialized.");
}

void loop()
{
    pcf8574bus.readState();

    atEngine.loop();

#ifdef TEST_MODE

    router->processRoutes(routes);

#else
    auto now = clock.now();
    solarSwitch.loop(now);

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
