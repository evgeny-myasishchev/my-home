#include <at-commands.h>

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

} // namespace v2