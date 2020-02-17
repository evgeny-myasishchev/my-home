#include <arduino-compat.h>
#include <logger.h>
#include <pin-bus.h>
#include <switch-service-v2.h>
#include <switches-router-v2.h>
#include <at.h>

// Test mode will only use pin bus, see below
// #define TEST_MODE

#include "routes/test-routes.h"
// #include "routes/fl-2-routes.h"

using namespace v2;

PCF8574Bus bus(RELAY_BOARDS, INPUT_BOARDS, true);

SwitchesRouter *router;

ArrayPtr<Switch *> routes = createRoutes();

io::SerialTextStream atStream(&Serial);
at::Engine atEngine(&atStream);

class ATPong : public at::Handler
{
public:
    ATPong() : at::Handler("AT+PING") {}
    void Handle(at::Input input, at::Responder *resp)
    {
        resp->writeLine("PONG");
        resp->writeOk();
    };
} atPong;

class ATLed : public at::Handler
{
public:
    ATLed() : at::Handler("AT+LED") {}
    void Handle(at::Input input, at::Responder *resp)
    {
        bool ok = false;
        if (input.length == 2 && strncasecmp("ON", input.body, input.length) == 0)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            ok = true;
        }
        else if (input.length == 3 && strncasecmp("OFF", input.body, input.length) == 0)
        {
            digitalWrite(LED_BUILTIN, LOW);
            ok = true;
        }
        if (ok)
        {
            resp->writeOk();
        }
        else
        {
            resp->writeError();
        }
    };
} atLed;

class ATEcho : public at::Handler
{
public:
    ATEcho() : at::Handler("AT+ECHO") {}
    void Handle(at::Input input, at::Responder *resp)
    {
        resp->writeLine(input.body, input.length);
        resp->writeOk();
    };
} atEcho;

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

    bus.setup(0xFF);

    atEngine.addCommandHandler(&atPong);
    atEngine.addCommandHandler(&atEcho);
    atEngine.addCommandHandler(&atLed);
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
            bus.setPin(relayIndex * 8 + bit, relaySwitchState);
        }
    }

    const byte switchAllState = bus.getPin((RELAY_BOARDS + INPUT_BOARDS - 1) * 8);
    if (switchAllState == LOW)
    {
        for (size_t i = 0; i < RELAY_BOARDS * 8; i++)
        {
            bus.setPin(i, LOW);
        }
    }

#endif

    bus.writeState();
}
