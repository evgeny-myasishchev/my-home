#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "test-lib/random.h"
#include "test-lib/test-text-stream.h"
#include "at-commands.h"

namespace
{

TEST(v2ATPing, Handle)
{
    v2::ATPing cmd;
    TestTextStream testStream;
    at::Responder responder(&testStream);

    ASSERT_EQ("AT+PING", cmd.Name());
    cmd.Handle(at::Input(), &responder);
    ASSERT_EQ("+PONG\nOK\n", testStream.writeBuffer);
}

} // namespace
