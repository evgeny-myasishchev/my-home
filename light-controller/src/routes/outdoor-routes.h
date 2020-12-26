#include <solar-switch.h>
#include <switch.h>

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 2

ArrayPtr<Switch *> createRoutes(SolarSwitch *solarSwitch)
{

    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch **routesArray = new Switch *[4];
    byte routeNumber = 0;
    Switch* route;

    // virtual
    const auto virtualAddressStart = relaysCount + switchesCount;
    solarSwitch->setPin(virtualAddressStart, 0, 1);
    solarSwitch->setSunriseOffsetMinutes(0);
    solarSwitch->setSunriseOffsetMinutes(0);

    // =================== Barn ===================
    // barn
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 0)
        ->withSwitchType(SwitchType::Push)
        ->withTargetAddresses(1, new byte[1]{8});
    routesArray[routeNumber++] = route;
    
    // shed normal
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 2)
        ->withSwitchType(SwitchType::Push)
        ->withTargetAddresses(1, new byte[1]{9});
    routesArray[routeNumber++] = route;

    // night land + shed
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 1)
        ->withSwitchType(SwitchType::Toggle)
        ->withTargetAddresses(2, new byte[2]{0, 3});
    routesArray[routeNumber++] = route;
    
    // night land + shed (from solar)
    route = (new Switch())
        ->withSwitchAddress(virtualAddressStart + 0)
        ->withSwitchType(SwitchType::Toggle)
        ->withTargetAddresses(2, new byte[2]{0, 3});
    routesArray[routeNumber++] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}