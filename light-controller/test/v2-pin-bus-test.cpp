#include <gtest/gtest.h>
#include <arduino-compat.h>
#include "test-lib/random.h"
#include <pin-bus.h>
#include "test-lib/test-pin-bus.h"

namespace
{

    TEST(V2PinBusPersistable, initialState)
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

    TEST(V2PinBusPersistable, getPin)
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

    TEST(V2PinBusPersistable, setPin)
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

    TEST(V2PinBusComposite, getBusSizeBytesSingleTarget)
    {
        const byte maxBytes = test::randomNumber(5, 10);
        const TestPinBus target1(maxBytes);
        v2::PinBus* targets[1] = {(v2::PinBus*)&target1};
        const v2::CompositePinBus composite(1, targets);
        const byte got = composite.getBusSizeBytes();
        ASSERT_EQ(maxBytes, got);
    }

    TEST(V2PinBusComposite, getBusSizeBytesMultipleTarget)
    {
        const byte target1Bytes = test::randomNumber(5, 10);
        const TestPinBus target1(target1Bytes);
        const byte target2Bytes = test::randomNumber(5, 10);
        const TestPinBus target2(target2Bytes);
        v2::PinBus* targets[2] = {(v2::PinBus*)&target1, (v2::PinBus*)&target2};
        const v2::CompositePinBus composite(2, targets);
        const byte got = composite.getBusSizeBytes();
        ASSERT_EQ(target1Bytes + target2Bytes, got);
    }

} // namespace
