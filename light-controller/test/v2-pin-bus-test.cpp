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
        v2::PinBus *targets[1] = {(v2::PinBus *)&target1};
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
        v2::PinBus *targets[2] = {(v2::PinBus *)&target1, (v2::PinBus *)&target2};
        const v2::CompositePinBus composite(2, targets);
        const byte got = composite.getBusSizeBytes();
        ASSERT_EQ(target1Bytes + target2Bytes, got);
    }

    TEST(V2PinBusComposite, getPin)
    {
        // bus1
        const byte target1Bytes = test::randomNumber(5, 10);
        byte *state1 = new byte[target1Bytes]();
        for (size_t byteIndex = 0; byteIndex < target1Bytes; byteIndex++)
        {
            state1[byteIndex] = test::randomNumber(0, 255);
        }
        TestPinBus target1(target1Bytes);
        target1.pendingTestState = state1;
        target1.readState();

        // bus2
        const byte target2Bytes = test::randomNumber(5, 10);
        byte *state2 = new byte[target2Bytes]();
        for (size_t byteIndex = 0; byteIndex < target2Bytes; byteIndex++)
        {
            state2[byteIndex] = test::randomNumber(0, 255);
        }
        TestPinBus target2(target2Bytes);
        target2.pendingTestState = state2;
        target2.readState();

        // bus3
        const byte target3Bytes = test::randomNumber(5, 10);
        byte *state3 = new byte[target3Bytes]();
        for (size_t byteIndex = 0; byteIndex < target3Bytes; byteIndex++)
        {
            state3[byteIndex] = test::randomNumber(0, 255);
        }
        TestPinBus target3(target3Bytes);
        target3.pendingTestState = state3;
        target3.readState();

        // composite bus
        v2::PinBus *targets[3] = {(v2::PinBus *)&target1, (v2::PinBus *)&target2, (v2::PinBus *)&target3};
        const v2::CompositePinBus composite(3, targets);

        // assert
        const auto bits1 = target1Bytes * 8;
        for (size_t i = 0; i < bits1; i++)
        {
            ASSERT_EQ(target1.getPin(i), composite.getPin(i)) << "Failed to validate first target";
        }

        const auto bits2 = target2Bytes * 8;
        for (size_t i = 0; i < bits2; i++)
        {
            ASSERT_EQ(target2.getPin(i), composite.getPin(bits1 + i)) << "Failed to validate second target. Target pin: " << i << ", composite pin: " << bits1 + i;
        }

        const auto bits3 = target3Bytes * 8;
        for (size_t i = 0; i < bits3; i++)
        {
            ASSERT_EQ(target3.getPin(i), composite.getPin(bits1 + bits2 + i)) << "Failed to validate third target. Target pin: " << i << ", composite pin: " << bits1 + bits2 + i;
        }
    }

    TEST(V2PinBusComposite, setPin)
    {
        // bus1
        const byte target1Bytes = test::randomNumber(5, 10);
        byte *target1State = new byte[target1Bytes]();
        TestPinBus target1(target1Bytes);
        target1.pendingTestState = target1State;
        target1.readState();

        // bus2
        const byte target2Bytes = test::randomNumber(5, 10);
        byte *target2State = new byte[target2Bytes]();
        TestPinBus target2(target2Bytes);
        target2.pendingTestState = target2State;
        target2.readState();

        // composite bus
        v2::PinBus *targets[2] = {(v2::PinBus *)&target1, (v2::PinBus *)&target2};
        v2::CompositePinBus composite(2, targets);

        // set state1
        byte *randomState1 = new byte[target1Bytes]();
        for (size_t byteIndex = 0; byteIndex < target1Bytes; byteIndex++)
        {
            const auto byteVal = test::randomNumber(0, 255);
            randomState1[byteIndex] = byteVal;
            for (size_t bit = 0; bit < 8; bit++)
            {
                composite.setPin((byteIndex * 8) + bit, bitRead(byteVal, bit));
            }
        }

        // set state2
        const auto target2PinsOffset = target1Bytes * 8;
        byte *randomState2 = new byte[target2Bytes]();
        for (size_t byteIndex = 0; byteIndex < target2Bytes; byteIndex++)
        {
            const auto byteVal = test::randomNumber(0, 255);
            randomState2[byteIndex] = byteVal;
            for (size_t bit = 0; bit < 8; bit++)
            {
                composite.setPin(target2PinsOffset + (byteIndex * 8) + bit, bitRead(byteVal, bit));
            }
        }

        // commit state
        target1.writeState();
        target2.writeState();

        // assert states
        for (size_t byteIndex = 0; byteIndex < target1Bytes; byteIndex++)
        {
            EXPECT_EQ(target1State[byteIndex], randomState1[byteIndex]);
        }
        for (size_t byteIndex = 0; byteIndex < target2Bytes; byteIndex++)
        {
            EXPECT_EQ(target2State[byteIndex], randomState2[byteIndex]);
        }
    }

} // namespace
