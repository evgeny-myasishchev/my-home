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

} // namespace v2

#endif