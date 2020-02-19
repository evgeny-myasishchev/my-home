#ifndef arduino_compat_h
#define arduino_compat_h

class DigitalWrite
{
public:
    virtual void fn(int pin, int value) = 0;
};

#ifndef ARDUINO

#include <stdint.h>
#include <stddef.h>

#define HIGH 0x1
#define LOW  0x0

typedef uint8_t byte;

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

class TestDigitalWrite : public DigitalWrite
{
public:
    int lastPin = 0;
    int lastValue = 0;
    void fn(int pin, int value)
    {
        lastPin = pin;
        lastValue = value;
    };
};

#else
#include <Arduino.h>
#endif

#endif