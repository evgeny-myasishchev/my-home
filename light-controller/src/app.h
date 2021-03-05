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
    // PCF8574Bus pcf8574bus = PCF8574Bus(RELAY_BOARDS, INPUT_BOARDS, true);
    PCF8574Bus *pcf8574bus;
    // Virtual bus is used by software switches (like solar)
    InMemoryPinBus virtualBus = InMemoryPinBus(1);
    PinBus **busses = new PinBus *[2] { pcf8574bus, &virtualBus };
    CompositePinBus bus = CompositePinBus((byte)2, busses);
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
    Dusk2Dawn location = Dusk2Dawn(50.04, 36.30, +2);
    rtc::ArduinoSolar solar = rtc::ArduinoSolar(&location);
    SolarSwitch solarSwitch = SolarSwitch(&solar, &bus);

    ArrayPtr<Switch *> routes;
public:
    App(byte relayBoards, byte inputBoards, ArrayPtr<Switch *> (*createRoutes)(SolarSwitch *solarSwitch));
    void setup();
    void loop();
};

#endif // APP_H
