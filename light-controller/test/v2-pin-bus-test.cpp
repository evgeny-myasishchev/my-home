#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "test-lib/random.h"
#include <pin-bus.h>
#include "test-lib/test-pin-bus.h"

namespace
{

TEST(V2PinBus, initialState)
{
    const byte maxBytes = test::randomNumber(5, 10);
    const TestPinBus bus(maxBytes);

    const byte totalPins = maxBytes * 8;
    for (size_t pinIndex = 0; pinIndex < totalPins; pinIndex++)
    {
        const auto pin = bus.getPin(pinIndex);
        EXPECT_EQ(LOW, pin);
    }
}

TEST(V2PinBus, getPin)
{
    const byte maxBytes = test::randomNumber(5, 10);
    byte *state = new byte[maxBytes]();
    for (size_t byteIndex = 0; byteIndex < maxBytes; byteIndex++)
    {
        state[byteIndex] = test::randomNumber(0, 255);
    }

    TestPinBus bus(maxBytes);
    bus.pendingTestState = state;
    bus.readState();

    const byte totalPins = maxBytes * 8;
    for (size_t pinIndex = 0; pinIndex < totalPins; pinIndex++)
    {

        const auto pin = bus.getPin(pinIndex);

        const byte byteIndex = pinIndex / 8;
        const byte bit = pinIndex % 8;

        EXPECT_EQ(bitRead(state[byteIndex], bit), pin);
    }
}

TEST(V2PinBus, setPin)
{
    const byte maxBytes = test::randomNumber(5, 10);
    byte *state = new byte[maxBytes]();
    TestPinBus bus(maxBytes);
    bus.pendingTestState = state;
    bus.readState();

    byte *randomState = new byte[maxBytes]();
    for (size_t byteIndex = 0; byteIndex < maxBytes; byteIndex++)
    {
        const auto byteVal = test::randomNumber(0, 255);
        randomState[byteIndex] = byteVal;
        for (size_t bit = 0; bit < 8; bit++)
        {
            bus.setPin((byteIndex * 8) + bit, bitRead(byteVal, bit));
        }
    }

    bus.writeState();

    for (size_t byteIndex = 0; byteIndex < maxBytes; byteIndex++)
    {
        EXPECT_EQ(state[byteIndex], randomState[byteIndex]);
    }
}

} // namespace
