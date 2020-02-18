#include "at-commands.h"

namespace v2
{

ATPing::ATPing() : at::Handler("AT+PING") {}

void ATPing::Handle(at::Input input, at::Responder *resp)
{
    resp->writeLine("PONG");
    resp->writeOk();
};

} // namespace v2