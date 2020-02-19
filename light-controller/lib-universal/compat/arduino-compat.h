#ifndef arduino_compat_h
#define arduino_compat_h

#include <stdint.h>

class DigitalWrite
{
public:
    virtual void fn(uint8_t pin, uint8_t val) = 0;
};

#ifndef ARDUINO

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
    uint8_t lastPin = 0;
    uint8_t lastValue = 0;
    void fn(uint8_t pin, uint8_t val)
    {
        lastPin = pin;
        lastValue = val;
    };
};

#else
#include <Arduino.h>

class ArduinoDigitalWrite : public DigitalWrite
{
private:
    void (*_digitalWrite)(uint8_t pin, uint8_t val);
public:
    ArduinoDigitalWrite(void (*digitalWrite)(uint8_t pin, uint8_t val))
    {
        _digitalWrite = digitalWrite;
    };

    void fn(uint8_t pin, uint8_t val)
    {
        _digitalWrite(pin, val);
    };
};
#endif

#endif