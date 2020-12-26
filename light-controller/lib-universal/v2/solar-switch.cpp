#include "solar-switch.h"

namespace v2
{

    SolarSwitch::SolarSwitch(rtc::Solar *solar, PinBus *bus)
    {
        _solar = solar;
        _bus = bus;
    }

    void SolarSwitch::setup(DateTime now)
    {
        auto sunriseOffsetMins = _solar->sunriseOffsetMinutes(now) + _sunrizeOffsetMinutes;
        auto sunsetOffsetMins = _solar->sunsetOffsetMinutes(now) + _sunsetOffsetMinutes;

        solar_switch_log("Solar switch setup: now=%d:%d:%d %d:%d:%d, effective sunrise offset=%d:%d, effective sunset offset=%d:%d, pin=%d, day state=%d, night state=%d", 
            now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
            sunriseOffsetMins / 60, sunriseOffsetMins % 60,
            sunsetOffsetMins / 60, sunsetOffsetMins % 60,
            _pinIndex, _dayState, _nightState
        );
    }

    void SolarSwitch::loop(DateTime now)
    {
        auto sunriseOffsetMins = _solar->sunriseOffsetMinutes(now) + _sunrizeOffsetMinutes;
        auto sunsetOffsetMins = _solar->sunsetOffsetMinutes(now) + _sunsetOffsetMinutes;
        auto nowOffsetMins = now.hour() * 60 + now.minute();

        auto state = _nightState;

        if (nowOffsetMins >= sunriseOffsetMins && nowOffsetMins <= sunsetOffsetMins)
        {
            state = _dayState;
        }
        const auto currentState = _bus->getPin(_pinIndex);
        if (currentState != state)
        {
            solar_switch_log("Solar state changed. Now=%d:%d:%d %d:%d:%d, effective sunrise offset: %d:%d, effective sunset offset: %d:%d, pin=%d, old=%d, new=%d",
                       now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
                       sunriseOffsetMins / 60, sunriseOffsetMins % 60,
                       sunsetOffsetMins / 60, sunsetOffsetMins % 60,
                       _pinIndex, currentState, state);
            _bus->setPin(_pinIndex, state);
        }
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