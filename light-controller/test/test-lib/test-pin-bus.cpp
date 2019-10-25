#include "test-pin-bus.h"

TestPinBus::TestPinBus(const byte busSize) : PinBus(busSize)
{
}

TestPinBus::~TestPinBus()
{
}

void TestPinBus::readState()
{
    for (size_t i = 0; i < this->getBusSize(); i++)
    {
        logger_log("Reading test pin bus state byte: %d, value: %d", i, pendingTestState[i]);
        this->setStateByte(i, pendingTestState[i]);
    }
}

void TestPinBus::writeState()
{
    for (size_t i = 0; i < this->getBusSize(); i++)
    {
        pendingTestState[i] = this->getStateByte(i);
    }
}