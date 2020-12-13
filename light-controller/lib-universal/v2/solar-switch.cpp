#include "solar-switch.h"

namespace v2
{

    SolarSwitch::SolarSwitch(rtc::Solar *solar, PinBus *bus)
    {
    }

    void SolarSwitch::loop(DateTime now)
    {
    }

    SolarSwitch *SolarSwitch::setSunriseOffsetMinutes(const uint8_t offset)
    {
        return this;
    }

    SolarSwitch *SolarSwitch::setSunsetOffsetMinutes(const uint8_t offset)
    {
        return this;
    }

    SolarSwitch *SolarSwitch::setPin(const byte pinIndex, const byte dayState, const byte nightState)
    {
        return this;
    }

} // namespace v2