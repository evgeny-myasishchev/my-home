#include <switch.h>

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 2

ArrayPtr<Switch *> createRoutes()
{
    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch **routesArray = new Switch *[switchesCount];
    byte routeNumber = 0;
    Switch* route;

    // =================== Barn ===================
    // barn + shed
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 0)
        ->withSwitchType(SwitchType::Toggle)
        ->withTargetAddresses(2, new byte[2]{2, 5});
    routesArray[routeNumber++] = route;

    // land night
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 1)
        ->withSwitchType(SwitchType::Toggle)
        ->withTargetAddresses(1, new byte[1]{0});
    routesArray[routeNumber++] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}