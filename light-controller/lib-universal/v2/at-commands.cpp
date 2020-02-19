#include <at-commands.h>
#include <string.h>

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

} // namespace v2