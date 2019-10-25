#ifndef V2_SWITCH
#define V2_SWITCH

#include <arduino-compat.h>
#include <array-ptr.h>

namespace v2
{

enum SwitchType
{
    Push = 0, // Will change state when switch pushed
    // TODO
    // PushOff = 1, // Same as Push but always do off
    Toggle = 2, // Will keep exact state as switch state
};

struct Switch
{
    SwitchType type;
    byte state;
    byte pendingState;
    bool stateChanged;
    unsigned int seenSignalTimes;
    unsigned int seenSignalSince;

    byte switchAddress;
    ArrayPtr<byte> targetAddresses;

    Switch *withSwitchType(SwitchType type)
    {
        this->type = type;
        return this;
    }

    Switch *withSwitchAddress(byte addr)
    {
        this->switchAddress = addr;
        return this;
    }

    Switch *withTargetAddresses(byte size, byte* addresses)
    {
        this->targetAddresses = ArrayPtr<byte>(size, addresses);
        return this;
    }
};

} // namespace v2

#endif