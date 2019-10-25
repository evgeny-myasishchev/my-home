#include <pin-bus.h>

namespace v2
{

#ifdef PIN_BUS_VERBOSE
#define pin_bus_log logger_log
#else
#define pin_bus_log
#endif

PinBus::PinBus(const byte busSize)
{
    this->busSize = busSize;
    this->busState = new byte[busSize]();
}

PinBus::~PinBus()
{
    delete this->busState;
}

const byte PinBus::getBusSize()
{
    return busSize;
}

void PinBus::setStateByte(const byte byteIndex, const byte state)
{
    if (byteIndex >= this->busSize)
    {
        return;
    }
    this->busState[byteIndex] = state;
}

const byte PinBus::getStateByte(const byte byteIndex)
{
    return this->busState[byteIndex];
}

const byte PinBus::getPin(const byte pinIndex) const
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

const void PinBus::setPin(const byte pinIndex, byte state)
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
    : outputBoardsNum(outputBoardsNum), inputBoardsNum(inputBoardsNum), PinBus(outputBoardsNum + inputBoardsNum)
{
    const auto busSize = this->getBusSize();
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

void PCF8574Bus::setup(const byte initialState)
{
    for (size_t i = 0; i < this->getBusSize(); i++)
    {
        boards[i]->begin();
    }
    pin_bus_log("Initializing %d bus boards. Initial state: %d", this->getBusSize(), initialState);
}

void PCF8574Bus::readState()
{
    for (size_t i = 0; i < this->getBusSize(); i++)
    {
        const byte byteValue = invert ? ~boards[i]->read8() : boards[i]->read8();
        this->setStateByte(i, byteValue);
        if (this->prevBusState[i] != byteValue)
        {
            pin_bus_log("Loaded board byte: %d, value: %b", i, byteValue);
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