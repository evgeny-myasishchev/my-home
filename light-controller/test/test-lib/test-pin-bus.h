#ifndef MOCKS_TEST_PIN_BUS
#define MOCKS_TEST_PIN_BUS

#include <pin-bus.h>

class TestPinBus : public v2::PinBus
{
public:
    byte *pendingTestState;

    TestPinBus(const byte busSize);
    ~TestPinBus();
    void readState();
    void writeState();
};

#endif