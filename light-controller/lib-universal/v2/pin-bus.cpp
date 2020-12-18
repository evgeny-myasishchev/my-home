#include "pin-bus.h"

namespace v2
{

#ifdef PIN_BUS_VERBOSE
#define pin_bus_log logger_log
#else
#define pin_bus_log
#endif

CompositePinBus::CompositePinBus(const byte targetsCount, PinBus **targets)
{
    for (size_t i = 0; i < targetsCount; i++)
    {
        const auto target = targets[i];
        totalSize = totalSize + target->getBusSizeBytes();
    }
    byteSlots = new PinBus*[totalSize];
    byteSlots[0] = targets[0];
}

CompositePinBus::~CompositePinBus()
{
    delete byteSlots;
}

const byte CompositePinBus::getPin(const byte pinIndex) const
{
    return byteSlots[0]->getPin(pinIndex);
}

const void CompositePinBus::setPin(const byte pinIndex, byte state)
{
}

const byte CompositePinBus::getBusSizeBytes() const
{
    return totalSize;
}

PersistablePinBus::PersistablePinBus(const byte busSize)
{
    this->busSize = busSize;
    this->busState = new byte[busSize]();
}

PersistablePinBus::~PersistablePinBus()
{
    delete this->busState;
}

const byte PersistablePinBus::getBusSizeBytes() const
{
    return busSize;
}

void PersistablePinBus::setStateByte(const byte byteIndex, const byte state)
{
    if (byteIndex >= this->busSize)
    {
        return;
    }
    this->busState[byteIndex] = state;
}

const byte PersistablePinBus::getStateByte(const byte byteIndex)
{
    return this->busState[byteIndex];
}

const byte PersistablePinBus::getPin(const byte pinIndex) const
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

const void PersistablePinBus::setPin(const byte pinIndex, byte state)
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
    : outputBoardsNum(outputBoardsNum), inputBoardsNum(inputBoardsNum), PersistablePinBus(outputBoardsNum + inputBoardsNum)
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
        boards[i]->begin(boardState);
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