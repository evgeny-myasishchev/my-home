#include <gtest/gtest.h>
#include <array-ptr.h>
#include "test-lib/test-text-stream.h"
#include "at.h"

namespace
{

TEST(atResponder, writeOk)
{
    TestTextStream testStream;
    at::Responder responder(&testStream);
    responder.writeOk();

    ASSERT_STREQ("OK\n", testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen("OK\n"), testStream.lastWrittenBufferLength);
}

TEST(atResponder, writeError)
{
    TestTextStream testStream;
    at::Responder responder(&testStream);
    responder.writeError();

    ASSERT_STREQ("ERROR\n", testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen("ERROR\n"), testStream.lastWrittenBufferLength);
}

TEST(atResponder, writeLine)
{
    TestTextStream testStream;
    at::Responder responder(&testStream);
    const char line[] = "SOME DATA LINE";
    responder.writeLine(line);

    char want[20] = "+";
    strcat(want, line);
    strcat(want, "\n");

    ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);
}

} // namespace
