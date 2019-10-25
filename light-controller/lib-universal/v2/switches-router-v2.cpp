#include <switches-router-v2.h>

namespace v2
{
SwitchesRouter::SwitchesRouter(SwitchRouterServices services)
{
    this->services = services;
}

void SwitchesRouter::processRoutes(ArrayPtr<Switch *> routes)
{
    for (size_t i = 0; i < routes.size(); i++)
    {
        Switch *sw = routes[i];
        SwitchService *swSvc = NULL;
        switch (sw->type)
        {
        case SwitchType::Push:
            swSvc = this->services.pushBtnSwitchSvc;
            break;
        case SwitchType::Toggle:
            swSvc = this->services.toggleBtnSwitchSvc;
            break;
        default:
            break;
        }
        if (swSvc != NULL)
        {
            const auto switchState = this->services.bus->getPin(sw->switchAddress);
            // router_log("Processing switch: %d, address: %d, state: %d", i, sw->switchAddress, switchState);
            swSvc->processSignal(switchState, sw);
            if (sw->stateChanged)
            {
                router_log("Route %d status changed to %d", i, sw->state);
                for (byte addrNum = 0; addrNum < sw->targetAddresses.size(); addrNum++)
                {
                    const auto address = sw->targetAddresses[addrNum];
                    const byte currentTargetValue = this->services.bus->getPin(address);
                    const byte newTargetValue = swSvc->getTargetState(currentTargetValue, sw);
                    router_log("State transition from %d to %d for %d address (addr num %d)", currentTargetValue, newTargetValue, address, addrNum);
                    this->services.bus->setPin(address, newTargetValue);
                }
                swSvc->applyStateChange(sw);
            }
        }
        else
        {
            logger_log("Unexpected switch type: %d at index: %d", sw->type, i);
        }
    }
}

} // namespace v2