#include <gtest/gtest.h>
#include <array-ptr.h>
#include "test-lib/test-text-stream.h"
#include "test-lib/test-at-handler.h"
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
    engine.setup();

    testStream.readBuffer.assign("AT\n");

    engine.loop();

    char want[] = "OK\n";

    ASSERT_EQ(want, testStream.writeBuffer);
}

TEST(atEngine, handleUnknownCommand)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);
    engine.setup();
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

TEST(atEngine, handleCommandNoInput)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);
    
    TestATHandler cmd1("AT+CMD1", "CMD1-RESPONSE");
    TestATHandler cmd2("AT+CMD2", "CMD2-RESPONSE");
    TestATHandler cmd3("AT+CMD3", "CMD3-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.addCommandHandler(&cmd2);
    engine.addCommandHandler(&cmd3);
    engine.setup();

    testStream.readBuffer.assign("AT+CMD1\n");
    engine.loop();

    ASSERT_EQ("", cmd1.gotInput);
    ASSERT_TRUE(cmd1.called) << "cmd1 not called";
    char *want = "+CMD1-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT+CMD2\n");
    engine.loop();

    ASSERT_EQ("", cmd2.gotInput);
    ASSERT_TRUE(cmd2.called) << "cmd2 not called";
    want = "+CMD2-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT+CMD3\n");
    engine.loop();

    ASSERT_EQ("", cmd3.gotInput);
    ASSERT_TRUE(cmd3.called) << "cmd3 not called";
    want = "+CMD3-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
}

TEST(atEngine, handleCommandWithInput)
{
    GTEST_SKIP();
    TestTextStream testStream;
    at::Engine engine(&testStream);
    
    TestATHandler cmd1("AT+CMD1", "CMD1-RESPONSE");
    TestATHandler cmd2("AT+CMD2", "CMD2-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.addCommandHandler(&cmd2);
    engine.setup();

    testStream.readBuffer.assign("AT+CMD1=CMD1-INPUT\n");
    engine.loop();

    ASSERT_EQ("CMD1-INPUT", cmd1.gotInput);
    ASSERT_TRUE(cmd1.called) << "cmd1 not called";
    char *want = "+CMD1-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT+CMD2=CMD2-INPUT\r\n");
    engine.loop();

    ASSERT_EQ("CMD2-INPUT", cmd2.gotInput);
    ASSERT_TRUE(cmd2.called) << "cmd2 not called";
    want = "+CMD2-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();
}

TEST(atEngine, handleChunkedWrites)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);

    TestATHandler cmd1("AT+CMD1", "CMD1-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.setup();

    testStream.readBuffer.assign("A");
    engine.loop();
    testStream.readBuffer.assign("T");
    engine.loop();
    testStream.readBuffer.assign("\n");
    engine.loop();

    char *want = "OK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT");
    engine.loop();
    testStream.readBuffer.assign("+CMD1");
    engine.loop();
    testStream.readBuffer.assign("\n");
    engine.loop();

    ASSERT_EQ(cmd1.gotInput, "");
    ASSERT_TRUE(cmd1.called) << "cmd1 not called";
    want = "+CMD1-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();
}

TEST(atEngine, ignoreCR)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);

    TestATHandler cmd1("AT+CMD1", "CMD1-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.setup();

    testStream.readBuffer.assign("AT\r\n");
    engine.loop();

    char *want = "OK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();

    testStream.readBuffer.assign("AT+CMD1\r\n");
    engine.loop();

    ASSERT_EQ(cmd1.gotInput, "");
    ASSERT_TRUE(cmd1.called) << "cmd1 not called";
    want = "+CMD1-RESPONSE\nOK\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();
}

TEST(atEngine, errorIfLargeCommand)
{
    TestTextStream testStream;
    at::Engine engine(&testStream);

    TestATHandler cmd1("AT+CMD1", "CMD1-RESPONSE");

    engine.addCommandHandler(&cmd1);
    engine.setup();

    testStream.readBuffer.assign("012345678901234567890123456789");
    engine.loop();
    testStream.readBuffer.assign("0123456789");
    engine.loop();

    char *want = "ERROR\n";
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();
}

} // namespace
