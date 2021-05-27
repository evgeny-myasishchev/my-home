#include <solar-switch.h>
#include <switch.h>

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 2

#define SPOT_ADDR_LAND_NIGHT 0
#define SPOT_ADDR_LAND_PAVILION 1 // This is temporary land night
#define SPOT_ADDR_SHED_COM 2
#define SPOT_ADDR_SHED_NIGHT_1 3
#define SPOT_ADDR_SHED_RESERVE_1 4
#define SPOT_ADDR_BARN_COM 5

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
        ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_BARN_COM});
    routesArray[routeNumber++] = route;

    // // pavilion (broken wire)
    // route = (new Switch())
    //     ->withSwitchAddress(relaysCount + 1)
    //     ->withSwitchType(SwitchType::Toggle)
    //     ->withTargetAddresses(1, new byte[2]{SPOT_ADDR_LAND_PAVILION});
    // routesArray[routeNumber++] = route;
    
    // shed normal
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 2)
        ->withSwitchType(SwitchType::Push)
        ->withTargetAddresses(2, new byte[2]{SPOT_ADDR_SHED_COM, SPOT_ADDR_LAND_PAVILION});
    routesArray[routeNumber++] = route;

    // night land + shed
    // route = (new Switch())
    //     ->withSwitchAddress(relaysCount + 1)
    //     ->withSwitchType(SwitchType::Toggle)
    //     ->withTargetAddresses(2, new byte[2]{0, 3});
    // routesArray[routeNumber++] = route;
    
    // night land + shed (from solar)
    route = (new Switch())
        ->withSwitchAddress(virtualAddressStart + 0)
        ->withSwitchType(SwitchType::Toggle)
        ->withTargetAddresses(2, new byte[2]{SPOT_ADDR_LAND_NIGHT, SPOT_ADDR_SHED_NIGHT_1});
    routesArray[routeNumber++] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}