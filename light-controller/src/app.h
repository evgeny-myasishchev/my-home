#ifndef APP_H
#define APP_H

#include <Dusk2Dawn.h>
#include <arduino-compat.h>
#include <logger.h>
#include <pin-bus.h>
#include <rtc-solar.h>
#include <switch-service-v2.h>
#include <switches-router-v2.h>
#include <solar-switch.h>
#include <Wire.h>
#include <at.h>
#include <at-commands.h>

// Test mode will only use pin bus, see below
// #define TEST_MODE

using namespace v2;

class App
{
private:
    PCF8574Bus *pcf8574bus;
    // Virtual bus is used by software switches (like solar)
    InMemoryPinBus virtualBus = InMemoryPinBus(1);
    CompositePinBus *bus;
    SwitchesRouter *router;

#ifdef AT_ENABLED
    at::SerialTextStream atStream = at::SerialTextStream(&Serial);
    at::Engine atEngine = at::Engine(&atStream);
    ArduinoDigitalWrite arduinoDigitalWrite = ArduinoDigitalWrite(digitalWrite);
    ATPing atPing;
    ATLed atLed = ATLed(LED_BUILTIN, &arduinoDigitalWrite);
    ATGetPin * atGetPin;
    ATSetPin * atSetPin;
#endif

    rtc::RTCClock *clock = new rtc::RTCClock();
    Dusk2Dawn *location = new Dusk2Dawn(50.04, 36.30, +3);
    rtc::ArduinoSolar *solar = new rtc::ArduinoSolar(location);
    SolarSwitch *solarSwitch;

    ArrayPtr<Switch *> routes;
public:
    App(byte relayBoards, byte inputBoards, ArrayPtr<Switch *> (*createRoutes)(SolarSwitch *solarSwitch));
    ~App();
    void setup();
    void loop();
};

#endif // APP_H
