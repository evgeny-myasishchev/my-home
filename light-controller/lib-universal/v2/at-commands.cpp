#include "at-commands.h"

namespace v2
{

ATPing::ATPing() : at::Handler("AT+PING") {}

void ATPing::Handle(at::Input input, at::Responder *resp)
{
    if (input.length == 0)
    {
        resp->writeLine("PONG");
    }
    else
    {
        resp->writeLine(input.body, input.length);
    }

    resp->writeOk();
};

ATLed::ATLed(const int pin, DigitalWrite *digitalWrite) : _pin(pin), at::Handler("AT+LED")
{
    this->_digitalWrite = digitalWrite;
}

void ATLed::Handle(at::Input input, at::Responder *resp)
{
    bool ok = false;
    int state = LOW;
    if (input.length == 2 && strncmp("ON", input.body, input.length) == 0)
    {
        state = HIGH;
        ok = true;
    }
    else if (input.length == 3 && strncmp("OFF", input.body, input.length) == 0)
    {
        state = LOW;
        ok = true;
    }
    if (ok)
    {
        _digitalWrite->fn(_pin, state);
        resp->writeOk();
    }
    else
    {
        resp->writeError();
    }
};

ATGetPin::ATGetPin(PinBus *bus) : at::Handler("AT+PIN?")
{
    _bus = bus;
};

void ATGetPin::Handle(at::Input input, at::Responder *resp)
{
    // TODO: Error if bad input
    // TODO: Error if index too large
    if(input.length > 0)
    {
        auto pinIndex = parseNumber(input.body, input.length);
        auto pinState = _bus->getPin((byte)pinIndex.value);
        resp->write(pinState);
        resp->write('\n');
        resp->writeOk();
    }
    else
    {
        resp->writeError();
    }
};

ATSetPin::ATSetPin(PinBus *bus) : at::Handler("AT+PIN")
{
    _bus = bus;
};

void ATSetPin::Handle(at::Input input, at::Responder *resp)
{
    // TODO: Error if bad input
    // TODO: Error if pin index too large
    // TODO: Error if state is not 0 or 1
    if(input.length > 0)
    {
        int comaIndex = 0;
        for (size_t i = 0; i < input.length; i++)
        {
            if(input.body[i] == ',')
            {
                comaIndex = i;
                break;
            }
        }

        auto pinIndex = parseNumber(input.body, comaIndex);
        auto pinState = parseNumber(&input.body[comaIndex+1], input.length);
        _bus->setPin(pinIndex.value, pinState.value);
        resp->writeOk();
    }
    else
    {
        resp->writeError();
    }
};

} // namespace v2