#ifndef V2_PIN_BUS_H
#define V2_PIN_BUS_H

#include <arduino-compat.h>
#include <logger.h>

#ifdef ARDUINO
#include <PCF8574.h>
#endif

namespace v2
{

#ifdef PIN_BUS_VERBOSE
#define pin_bus_log logger_log
#else
#define pin_bus_log
#endif

    class PinBus
    {
    public:
        const virtual byte getPin(const byte pinIndex) const = 0;
        const virtual void setPin(const byte pinIndex, byte state) = 0;
        const virtual byte getBusSizeBytes() const = 0;
    };

    struct CompositeBusSlot
    {
        PinBus *bus;
        int firstSlot;
    };

    class CompositePinBus : public PinBus
    {

    private:
        byte totalSize = 0;
        CompositeBusSlot *byteSlots;

    public:
        CompositePinBus(const byte targetsCount, PinBus **targets);
        ~CompositePinBus();
        const byte getPin(const byte pinIndex) const;
        const void setPin(const byte pinIndex, byte state);
        const byte getBusSizeBytes() const;
    };

    class InMemoryPinBus : public PinBus
    {
    private:
        byte *busState;
        byte busSize;

    protected:
        void setStateByte(const byte byteIndex, const byte state);
        const byte getStateByte(const byte byteIndex);

    public:
        InMemoryPinBus(const byte busSize);
        virtual ~InMemoryPinBus();
        const byte getBusSizeBytes() const;
        const byte getPin(const byte pinIndex) const;
        const void setPin(const byte pinIndex, byte state);
    };

#ifdef ARDUINO

#define PCF8574_BASE_ADDR 0x20

    // Bus implementation based on PCF8574 boards
    // Address space starts from output boards (e.g relays)
    // followed by input boards (e.g switches)
    class PCF8574Bus : public InMemoryPinBus
    {
    private:
        byte outputBoardsNum;
        byte inputBoardsNum;
        bool invert;

        PCF8574 **boards;

        // Used mostly to reduce logging noise
        byte *prevBusState;

    public:
        PCF8574Bus(
            const byte outputBoardsNum,
            const byte inputBoardsNum,

            // By default signals are HIGH off/no-signal
            // Invert flag will return HIGH when LOW
            // and write HIGH when LOW
            bool invert);
        ~PCF8574Bus();

        void setup(const byte outputState, const byte inputState);

        void readState();
        void writeState();
    };

#endif

} // namespace v2

#endif