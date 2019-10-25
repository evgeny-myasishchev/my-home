#include <switch-service-v2.h>

namespace v2
{

SwitchService::SwitchService(SwitchServiceConfig cfg)
{
    this->cfg = cfg;
    logger_log("Init switch service: minSignalDurationMs=%d, minSignalIterations=%d",
               cfg.minSignalDurationMs,
               cfg.minSignalIterations);
}

SwitchService::~SwitchService()
{
}

PushButtonSwitchService::PushButtonSwitchService(SwitchServiceConfig cfg) : SwitchService(cfg)
{
}

void PushButtonSwitchService::processSignal(uint8_t signal, Switch *sw)
{
    if (signal == HIGH)
    {
        service_log("Processing signal %d, seen times: %d, since: %d", signal, sw->seenSignalTimes, sw->seenSignalSince);
        sw->seenSignalTimes += 1;
        unsigned long now = cfg.timers->millis();
        if (sw->seenSignalSince == 0)
        {
            service_log("Seen signal for a first time, now is: %d", now);
            sw->seenSignalSince = now;
        };

        const unsigned int signalDuration = now - sw->seenSignalSince;

        if (sw->seenSignalTimes >= cfg.minSignalIterations &&
            signalDuration >= cfg.minSignalDurationMs &&
            sw->pendingState != signal)
        {
            sw->state = sw->pendingState = HIGH;
            sw->stateChanged = true;
            service_log("State change detected. Signal duration: %d, new state: %d", signalDuration, sw->state);
        }
    }
    else
    {
        sw->seenSignalTimes = 0;
        sw->seenSignalSince = 0;
        sw->pendingState = LOW;
    }
}

byte PushButtonSwitchService::getTargetState(byte currentValue, Switch *sw)
{
    if (sw->state == HIGH)
    {
        return currentValue == HIGH ? LOW : HIGH;
    }
    return currentValue;
}

void PushButtonSwitchService::applyStateChange(Switch *sw)
{
    sw->stateChanged = false;
    sw->state = LOW;
}

ToggleButtonSwitchService::ToggleButtonSwitchService(SwitchServiceConfig cfg) : SwitchService(cfg)
{
}

void ToggleButtonSwitchService::processSignal(byte signal, Switch *sw)
{
    unsigned long now = cfg.timers->millis();
    if (sw->state == signal)
    {
        return;
    }

    service_log("Processing signal %d, seen times: %d, since: %d", signal, sw->seenSignalTimes, sw->seenSignalSince);
    if (sw->pendingState == signal)
    {
        sw->seenSignalTimes += 1;

        const unsigned int signalDuration = now - sw->seenSignalSince;

        if (sw->seenSignalTimes >= cfg.minSignalIterations &&
            signalDuration >= cfg.minSignalDurationMs)
        {

            sw->stateChanged = true;
            sw->state = signal;
            service_log("State change detected. Signal duration: %d, new state: %d", signalDuration, sw->state);
        }
    }
    else
    {
        service_log("Seen %d signal for a first time, now is: %d", signal, now);
        sw->seenSignalSince = now;
        sw->seenSignalTimes = 1;
        sw->pendingState = signal;
    }
}

byte ToggleButtonSwitchService::getTargetState(byte currentValue, Switch *sw)
{
    return sw->state;
}

void ToggleButtonSwitchService::applyStateChange(Switch *sw)
{
    sw->stateChanged = false;
    sw->seenSignalTimes = 0;
    sw->seenSignalSince = 0;
}

} // namespace v2