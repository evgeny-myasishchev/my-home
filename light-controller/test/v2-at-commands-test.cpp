#include <iterator>
#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "test-lib/random.h"
#include "test-lib/test-text-stream.h"
#include "test-lib/test-pin-bus.h"
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
    cmd.Handle(at::Input((char *)&input, std::size(input) - 1), &responder); // -1 to skip null terminator
    ASSERT_EQ("+PING-INPUT\nOK\n", testStream.writeBuffer);
}


TEST(v2ATLed, HandleON)
{
    TestDigitalWrite digitalWrite;
    v2::ATLed cmd(32, &digitalWrite);
    TestTextStream testStream;
    at::Responder responder(&testStream);

    ASSERT_EQ("AT+LED", cmd.Name());
    cmd.Handle(at::Input("ON", 2), &responder);
    ASSERT_EQ("OK\n", testStream.writeBuffer);
    ASSERT_EQ(32, digitalWrite.lastPin);
    ASSERT_EQ(HIGH, digitalWrite.lastValue);
    testStream.reset();
}

TEST(v2ATLed, HandleOFF)
{
    TestDigitalWrite digitalWrite;
    v2::ATLed cmd(32, &digitalWrite);
    TestTextStream testStream;
    at::Responder responder(&testStream);

    cmd.Handle(at::Input("OFF", 3), &responder);
    ASSERT_EQ("OK\n", testStream.writeBuffer);
    ASSERT_EQ(32, digitalWrite.lastPin);
    ASSERT_EQ(LOW, digitalWrite.lastValue);
    testStream.reset();
}

TEST(v2ATLed, HandleUNKNOWN)
{
    TestDigitalWrite digitalWrite;
    v2::ATLed cmd(32, &digitalWrite);
    TestTextStream testStream;
    at::Responder responder(&testStream);

    cmd.Handle(at::Input("UNKNOWN-CMD", 11), &responder);
    ASSERT_EQ("ERROR\n", testStream.writeBuffer);
    ASSERT_EQ(0, digitalWrite.lastPin);
    testStream.reset();
}

TEST(v2ATGetPin, HandleNoInput)
{
    TestPinBus pinBus(2);
    v2::ATGetPin cmd(&pinBus);
    TestTextStream testStream;
    at::Responder responder(&testStream);

    cmd.Handle(at::Input(), &responder);
    ASSERT_EQ("ERROR\n", testStream.writeBuffer);
    testStream.reset();
}

TEST(v2ATGetPin, ReturnsPinValue)
{
    TestPinBus bus(2);
    const byte state = test::randomNumber(0, 255);
    const byte pin = test::randomNumber(0, 8);
    bus.pendingTestState[0] = state;
    bus.readState();
    v2::ATGetPin cmd(&bus);
    TestTextStream testStream;
    at::Responder responder(&testStream);

    const byte wantState = bus.getPin(pin);
    std::string want = "+PIN:" + std::to_string(wantState) + "\nOK\n";

    cmd.Handle(at::Input((char *)std::to_string(pin).c_str(), 1), &responder);
    ASSERT_EQ(want, testStream.writeBuffer);
    testStream.reset();
}

} // namespace
