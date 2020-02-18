#include <iterator>
#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "test-lib/random.h"
#include "test-lib/test-text-stream.h"
#include "at-commands.h"

namespace
{

TEST(v2ATPing, HandleNoInput)
{
    v2::ATPing cmd;
    TestTextStream testStream;
    at::Responder responder(&testStream);

    ASSERT_EQ("AT+PING", cmd.Name());
    cmd.Handle(at::Input(), &responder);
    ASSERT_EQ("+PONG\nOK\n", testStream.writeBuffer);
}

TEST(v2ATPing, EchoInput)
{
    v2::ATPing cmd;
    TestTextStream testStream;
    at::Responder responder(&testStream);

    const char input[] = "PING-INPUT";
    ASSERT_EQ("AT+PING", cmd.Name());
    cmd.Handle(at::Input((char*)&input, std::size(input) - 1), &responder); // -1 to skip null terminator
    ASSERT_EQ("+PING-INPUT\nOK\n", testStream.writeBuffer);
}

} // namespace
