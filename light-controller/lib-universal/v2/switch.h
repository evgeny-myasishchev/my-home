#ifndef V2_SWITCH
#define V2_SWITCH

#include <arduino-compat.h>
#include "array-ptr.h"

namespace v2
{

enum SwitchType
{
    Push = 0, // Will change state when switch pushed
    Toggle = 2, // Will keep exact state as switch state
};

enum SwitchAction
{
    Default = 0, // Standard action for a given switch
    ForceOff = 1, // Will always set state to off (low) if activated
};

struct Switch
{
    SwitchType type;
    SwitchAction action;
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

    Switch *withSwitchAction(SwitchAction action)
    {
        this->action = action;
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