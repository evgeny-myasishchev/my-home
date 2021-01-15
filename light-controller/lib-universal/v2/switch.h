#ifndef V2_SWITCH
#define V2_SWITCH

#include <arduino-compat.h>
#include "array-ptr.h"

namespace v2
{

#define PRIMARY_TARGET_UNDEFINED 255

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

    // if primary address is defined then it's state will be used
    // when toggling targets
    byte primaryTargetAddress = PRIMARY_TARGET_UNDEFINED;

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

    Switch *withPrimaryTargetAddress(byte primaryTargetAddress)
    {
        this->primaryTargetAddress = primaryTargetAddress;
        return this;
    }
};

} // namespace v2

#endif
