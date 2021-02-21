#ifndef V2_AT_COMMANDS
#define V2_AT_COMMANDS

#include <at.h>
#include <string.h>
#include <numerics.h>
#include "pin-bus.h"

namespace v2
{

class ATPing : public at::Handler
{
public:
    ATPing();
    void Handle(at::Input input, at::Responder *resp);
};

// AT+LED=ON|OFF
class ATLed : public at::Handler
{
private:
    const int _pin;
    DigitalWrite *_digitalWrite;
public:
    ATLed(const int pin, DigitalWrite *digitalWrite);
    void Handle(at::Input input, at::Responder *resp);
};

// AT+PIN?=<pinNumber>
class ATGetPin : public at::Handler
{
private:
    PinBus *_bus;
public:
    ATGetPin(PinBus *bus);
    void Handle(at::Input input, at::Responder *resp);
};

// AT+PIN=<pinNumber>,<state>
class ATSetPin : public at::Handler
{
private:
    PinBus *_bus;
public:
    ATSetPin(PinBus *bus);
    void Handle(at::Input input, at::Responder *resp);
};

} // namespace v2

#endif