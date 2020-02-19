#ifndef V2_AT_COMMANDS
#define V2_AT_COMMANDS

#include <at.h>

namespace v2
{

class ATPing : public at::Handler
{
public:
    ATPing();
    void Handle(at::Input input, at::Responder *resp);
};

class ATLed : public at::Handler
{
private:
    const int _pin;
    DigitalWrite *_digitalWrite;
public:
    ATLed(const int pin, DigitalWrite *digitalWrite);
    void Handle(at::Input input, at::Responder *resp);
    // {
    //     bool ok = false;
    //     if (input.length == 2 && strncasecmp("ON", input.body, input.length) == 0)
    //     {
    //         digitalWrite(LED_BUILTIN, HIGH);
    //         ok = true;
    //     }
    //     else if (input.length == 3 && strncasecmp("OFF", input.body, input.length) == 0)
    //     {
    //         digitalWrite(LED_BUILTIN, LOW);
    //         ok = true;
    //     }
    //     if (ok)
    //     {
    //         resp->writeOk();
    //     }
    //     else
    //     {
    //         resp->writeError();
    //     }
    // };
};

} // namespace v2

#endif