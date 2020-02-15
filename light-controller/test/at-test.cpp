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
}

} // namespace
