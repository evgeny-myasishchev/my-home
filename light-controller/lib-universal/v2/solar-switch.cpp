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

        uint8_t sunriseHour = sunrise.hour() + _sunrizeOffsetMinutes / 60;
        uint8_t sunriseMinute = sunrise.minute() + _sunrizeOffsetMinutes % 60;
        auto sunsetHour = sunset.hour() + _sunsetOffsetMinutes / 60;
        auto sunsetMinute = sunset.minute() + _sunsetOffsetMinutes % 60;

        if (
            (now.hour() >= sunriseHour && now.hour() <= sunsetHour) &&
            (now.minute()) >= sunriseMinute && now.minute() <= sunsetMinute)
        {
            state = _dayState;
        }
        _bus->setPin(_pinIndex, state);
    }

    void SolarSwitch::setSunriseOffsetMinutes(const uint8_t offset)
    {
        _sunrizeOffsetMinutes = offset;
    }

    void SolarSwitch::setSunsetOffsetMinutes(const uint8_t offset)
    {
        _sunsetOffsetMinutes = offset;
    }

    void SolarSwitch::setPin(const byte pinIndex, const byte dayState, const byte nightState)
    {
        _pinIndex = pinIndex;
        _dayState = dayState;
        _nightState = nightState;
    }

} // namespace v2