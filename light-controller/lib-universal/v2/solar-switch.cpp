#include "solar-switch.h"

namespace v2
{

    SolarSwitch::SolarSwitch(rtc::Solar *solar, PinBus *bus)
    {
        _solar = solar;
        _bus = bus;
    }

    void SolarSwitch::loop(DateTime now)
    {
        auto sunrise = _solar->sunrise();
        auto sunset = _solar->sunset();

        auto state = _nightState;

        if (
            (now.hour() >= sunrise.hour() && now.hour() <= sunset.hour()) &&
            (now.minute()) >= sunrise.minute() && now.hour() <= sunset.minute())
        {
            state = _dayState;
        }
        _bus->setPin(_pinIndex, state);
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
        _pinIndex = pinIndex;
        _dayState = dayState;
        _nightState = nightState;
        return this;
    }

} // namespace v2