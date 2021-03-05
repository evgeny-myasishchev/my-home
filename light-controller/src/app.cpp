#include "app.h"

App::App(byte relayBoards, byte inputBoards, ArrayPtr<Switch *> (*createRoutes)(SolarSwitch *solarSwitch))
{
     pcf8574bus = new PCF8574Bus(relayBoards, inputBoards, true);
     const auto busses = new PinBus *[2] { pcf8574bus, &virtualBus };
     bus = new CompositePinBus((byte)2, busses);
     solarSwitch = new SolarSwitch(solar, bus);
     routes = createRoutes(solarSwitch);
}

void App::setup()
{
    Wire.begin();

#ifndef DISABLE_LOGGING
    Serial.begin(115200);
    while (!Serial)
    {
    }
    logger_setup(clock, &Serial);
#endif

    router = new SwitchesRouter(SwitchRouterServices{
        .bus = bus,
        .pushBtnSwitchSvc = new PushButtonSwitchService(SwitchServiceConfig()),
        .toggleBtnSwitchSvc = new ToggleButtonSwitchService(SwitchServiceConfig()),
    });

    pcf8574bus->setup(0x00, 0x00);

    auto now = clock->now();
    solarSwitch->setup(now);

#ifdef AT_ENABLED
    atEngine.addCommandHandler(&atPing);
    atEngine.addCommandHandler(&atLed);
    atEngine.addCommandHandler(&atGetPin);
    atEngine.addCommandHandler(&atSetPin);
    atEngine.setup();
#endif

    logger_log("APP initialized.");
}

void App::loop()
{
    pcf8574bus->readState();

#ifdef AT_ENABLED
    atEngine.loop();
#endif

#ifndef TEST_MODE

    auto now = clock->now();
    solarSwitch->loop(now);
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

    pcf8574bus->writeState();
}