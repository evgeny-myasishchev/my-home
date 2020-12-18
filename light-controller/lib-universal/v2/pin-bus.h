#ifndef V2_PIN_BUS_H
#define V2_PIN_BUS_H

#include <arduino-compat.h>
#include <logger.h>

#ifdef ARDUINO
#include <PCF8574.h>
#endif

namespace v2
{

    class PinBus
    {
    public:
        const virtual byte getPin(const byte pinIndex) const = 0;
        const virtual void setPin(const byte pinIndex, byte state) = 0;
    };

    class CompositePinBus : public PinBus
    {
        CompositePinBus(const byte targetsCount, PinBus **targets);
    };

    class PersistablePinBus : public PinBus
    {
    private:
        byte *busState;
        byte busSize;

    protected:
        const byte getBusSize();
        void setStateByte(const byte byteIndex, const byte state);
        const byte getStateByte(const byte byteIndex);

    public:
        PersistablePinBus(const byte busSize);
        virtual ~PersistablePinBus();
        const byte getPin(const byte pinIndex) const;
        const void setPin(const byte pinIndex, byte state);

        // Read bus state from underlying implementation (e.g hardware)
        // and initialize internal state
        virtual void readState() = 0;

        // Write current state to underlying implementation (e.g hardware)
        virtual void writeState() = 0;
    };

#ifdef ARDUINO

#define PCF8574_BASE_ADDR 0x20

    // Bus implementation based on PCF8574 boards
    // Address space starts from output boards (e.g relays)
    // followed by input boards (e.g switches)
    class PCF8574Bus : public PersistablePinBus
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