#include "pin-bus.h"

namespace v2
{
    CompositePinBus::CompositePinBus()
    {
    }

    CompositePinBus::CompositePinBus(const byte targetsCount, PinBus **targets)
    {
        for (size_t i = 0; i < targetsCount; i++)
        {
            const auto target = targets[i];
            totalSize = totalSize + target->getBusSizeBytes();
        }
        byteSlots = new CompositeBusSlot[totalSize];
        byte slotIndex = 0;
        for (size_t i = 0; i < targetsCount; i++)
        {
            const auto target = targets[i];
            CompositeBusSlot slot;
            slot.bus = target;
            slot.firstSlot = slotIndex;
            for (size_t j = 0; j < target->getBusSizeBytes(); j++)
            {
                byteSlots[slotIndex] = slot;
                // printf("target: %zu, busByte: %zu, slot: %d, pinsFrom: %d, totalPins: %d\n", i, j, slotIndex, slotIndex * 8, target->getBusSizeBytes() * 8);
                slotIndex++;
            }
        }
    }

    CompositePinBus::~CompositePinBus()
    {
        delete byteSlots;
    }

    const byte CompositePinBus::getPin(const byte pinIndex) const
    {
        const byte slotIndex = pinIndex / 8;
        const auto slot = byteSlots[slotIndex];
        // printf("getPin: %d, slotIndex: %d, firstSlot: %d, actualPin: %d\n", pinIndex, slotIndex, slot.firstSlot, pinIndex - slot.firstSlot*8);
        return slot.bus->getPin(pinIndex - slot.firstSlot * 8);
    }

    const void CompositePinBus::setPin(const byte pinIndex, byte state)
    {
        const byte slotIndex = pinIndex / 8;
        const auto slot = byteSlots[slotIndex];
        // printf("setPin: %d, slotIndex: %d\n", pinIndex, slotIndex);
        slot.bus->setPin(pinIndex - slot.firstSlot * 8, state);
    }

    const byte CompositePinBus::getBusSizeBytes() const
    {
        return totalSize;
    }

    InMemoryPinBus::InMemoryPinBus(const byte busSize)
    {
        this->busSize = busSize;
        this->busState = new byte[busSize]();
    }

    InMemoryPinBus::~InMemoryPinBus()
    {
        delete this->busState;
    }

    const byte InMemoryPinBus::getBusSizeBytes() const
    {
        return busSize;
    }

    void InMemoryPinBus::setStateByte(const byte byteIndex, const byte state)
    {
        if (byteIndex >= this->busSize)
        {
            return;
        }
        this->busState[byteIndex] = state;
    }

    const byte InMemoryPinBus::getStateByte(const byte byteIndex)
    {
        return this->busState[byteIndex];
    }

    const byte InMemoryPinBus::getPin(const byte pinIndex) const
    {
        const byte byteIndex = pinIndex / 8;
        const byte bit = pinIndex % 8;
        if (byteIndex >= this->busSize)
        {
            return LOW;
        }
        const auto byte = this->busState[byteIndex];
        return bitRead(byte, bit);
    }

    const void InMemoryPinBus::setPin(const byte pinIndex, byte state)
    {
        const byte byteIndex = pinIndex / 8;
        const byte bit = pinIndex % 8;
        if (byteIndex >= this->busSize)
        {
            return;
        }
        bitWrite(this->busState[byteIndex], bit, state);
    }

#ifdef ARDUINO

    PCF8574Bus::PCF8574Bus(const byte outputBoardsNum, const byte inputBoardsNum, bool invert)
        : outputBoardsNum(outputBoardsNum), inputBoardsNum(inputBoardsNum), InMemoryPinBus(outputBoardsNum + inputBoardsNum)
    {
        const auto busSize = this->getBusSizeBytes();
        byte initialAddress = PCF8574_BASE_ADDR;
        boards = new PCF8574 *[busSize];
        for (size_t i = 0; i < busSize; i++)
        {
            const auto boardAddr = initialAddress++;
            boards[i] = new PCF8574(boardAddr);
        }

        // Skip init to have initial values logged
        prevBusState = new byte[busSize];

        this->invert = invert;
    }

    PCF8574Bus::~PCF8574Bus()
    {
        delete prevBusState;
        delete boards;
    }

    void PCF8574Bus::setup(const byte outputState, const byte inputState)
    {
        pin_bus_log("Initializing %d bus boards. Initial state: output: %d, input: %d", this->getBusSizeBytes(), outputState, inputState);
        for (size_t i = 0; i < this->getBusSizeBytes(); i++)
        {
            const byte rawBoardState = i < outputBoardsNum ? outputState : inputState;
            prevBusState[i] = rawBoardState;
            const byte boardState = invert ? ~rawBoardState : rawBoardState;
            boards[i]->write8(boardState);
            pin_bus_log("Board %d: state=%d", i, rawBoardState);
        }
    }

    void PCF8574Bus::readState()
    {
        for (size_t i = 0; i < this->getBusSizeBytes(); i++)
        {
            const byte rawByte = boards[i]->read8();
            const byte byteValue = invert ? ~rawByte : rawByte;
            this->setStateByte(i, byteValue);
            if (this->prevBusState[i] != byteValue)
            {
                pin_bus_log("Loaded board byte: %d, value(b): %b, value(d): %d, raw val(b): %b", i, byteValue, byteValue, rawByte);
            }
            this->prevBusState[i] = byteValue;
        }
    }

    void PCF8574Bus::writeState()
    {
        // Write only to outputs. Inputs should stay high so we could get a signal
        for (size_t i = 0; i < outputBoardsNum; i++)
        {
            const auto byteValue = this->getStateByte(i);
            if (this->prevBusState[i] != byteValue)
            {
                pin_bus_log("Write board byte: %d, value: %b", i, byteValue);
            }
            boards[i]->write8(invert ? ~byteValue : byteValue);
            this->prevBusState[i] = byteValue;
        }
    }

#endif

} // namespace v2