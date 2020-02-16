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

    ASSERT_EQ("OK\n", testStream.writeBuffer);
}

TEST(atResponder, writeError)
{
    TestTextStream testStream;
    at::Responder responder(&testStream);
    responder.writeError();

    ASSERT_EQ("ERROR\n", testStream.writeBuffer);
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

    ASSERT_EQ(want, testStream.writeBuffer);
}

TEST(atEngine, handleAT)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);

    testStream.readBuffer.assign("AT\n");

    engine.loop();

    char want[] = "OK\n";

    ASSERT_EQ(want, testStream.writeBuffer);
}

TEST(atEngine, handleUnknownCommand)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);
    const char want[] = "ERROR\n";

    testStream.readBuffer.assign("UNKNOWN\n");
    engine.loop();
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT1\n");
    engine.loop();
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("0AT\n");
    engine.loop();
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();
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
    TestTextStream testStream;
    at::Engine engine(&testStream);

    testHandler cmd1("AT+CMD1", "CMD1-RESPONSE");
    testHandler cmd2("AT+CMD2", "CMD2-RESPONSE");
    testHandler cmd3("AT+CMD3", "CMD3-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.addCommandHandler(&cmd2);
    engine.addCommandHandler(&cmd3);

    testStream.readBuffer.assign("AT+CMD1\n");
    engine.loop();

    ASSERT_STREQ(cmd1.gotInput, NULL);
    ASSERT_TRUE(cmd1.called) << "cmd1 not called";
    char *want = "+CMD1-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT+CMD2\n");
    engine.loop();

    // ASSERT_STREQ(cmd2.gotInput, NULL);
    // ASSERT_TRUE(cmd2.called) << "cmd2 not called";
    // want = "+CMD2-RESPONSE\nOK\n";
    // ASSERT_EQ(want, testStream.writeBuffer);
    // testStream.reset();

    // testStream.readBuffer.assign("AT+CMD3\n");
    // engine.loop();

    // ASSERT_STREQ(cmd3.gotInput, NULL);
    // ASSERT_TRUE(cmd3.called) << "cmd3 not called";
    // want = "+CMD3-RESPONSE\nOK\n";
    // ASSERT_EQ(want, testStream.writeBuffer);
}

} // namespace
