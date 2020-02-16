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

TEST(atEngine, handleAT)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);

    const char input[] = "AT\n";
    testStream.bufferToRead = input;
    testStream.bufferToReadLength = strlen(input);

    engine.loop();

    char want[] = "OK\n";

    ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);
}

TEST(atEngine, handleUnknownCommand)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);
    const char want[] = "ERROR\n";

    testStream.bufferToRead = "UNKNOWN\n";
    testStream.bufferToReadLength = strlen(testStream.bufferToRead);
    engine.loop();
    ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);

    testStream.bufferToRead = "AT1\n";
    testStream.bufferToReadLength = strlen(testStream.bufferToRead);
    engine.loop();
    ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);

    testStream.bufferToRead = "0AT\n";
    testStream.bufferToReadLength = strlen(testStream.bufferToRead);
    engine.loop();
    ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);
}

class testHandler : public at::Handler
{
private:
    const char *_response;
public:
    bool called = false;
    const char *gotInput = 0;
    testHandler(const char *name, const char *response) : at::Handler(name)
    {
        _response = response;
    }

    void Handle(const char *input, at::Responder *resp)
    {
        called = true;
        gotInput = input;
        resp->writeLine(_response);
        resp->writeOk();
    }
};

TEST(atEngine, handleCommandNoInput)
{
    GTEST_SKIP();

    TestTextStream testStream;
    at::Engine engine(&testStream);


    testHandler cmd1("CMD1", "CMD1-RESPONSE");
    testHandler cmd2("CMD2", "CMD2-RESPONSE");
    testHandler cmd3("CMD3", "CMD3-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.addCommandHandler(&cmd2);
    engine.addCommandHandler(&cmd3);

    testStream.setReadBuffer("AT+CMD1\n");
    engine.loop();

    ASSERT_STREQ(cmd1.gotInput, NULL);
    ASSERT_TRUE(cmd1.called) << "cmd1 not called";
    char *want = "CMD1-RESPONSE\nOK\n";
    ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);

    // testStream.setReadBuffer("AT+CMD2\n");
    // engine.loop();

    // ASSERT_STREQ(cmd2.gotInput, NULL);
    // ASSERT_TRUE(cmd2.called) << "cmd2 not called";
    // want = "CMD2-RESPONSE\nOK\n";
    // ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    // ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);

    // testStream.setReadBuffer("AT+CMD3\n");
    // engine.loop();

    // ASSERT_STREQ(cmd3.gotInput, NULL);
    // ASSERT_TRUE(cmd3.called) << "cmd3 not called";
    // want = "CMD3-RESPONSE\nOK\n";
    // ASSERT_STREQ(want, testStream.lastWrittenBuffer);
    // ASSERT_EQ(strlen(want), testStream.lastWrittenBufferLength);
}

} // namespace
