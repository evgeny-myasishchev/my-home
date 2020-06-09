#include <switch.h>

using namespace v2;

#define RELAY_BOARDS 3
#define INPUT_BOARDS 3

ArrayPtr<Switch *> createRoutes()
{
    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch **routesArray = new Switch *[switchesCount];
    byte routeNumber = 0;
    Switch* route;

    // for (byte switchNr = 0; switchNr < switchesCount; switchNr++)
    // for (byte switchNr = 0; switchNr < switchesCount - 1; switchNr++)
    // {
    //     Switch *route = (new Switch())
    //                         ->withSwitchAddress(switchesOffset + switchNr)
    //                         ->withTargetAddresses(1, new byte[1]{switchNr});
    //     routesArray[switchNr] = route;
    // }

    // =================== Office ===================
    // room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 0)
        ->withTargetAddresses(1, new byte[1]{2});
    routesArray[routeNumber++] = route;

    // under stairs
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 1)
        ->withTargetAddresses(1, new byte[1]{9});
    routesArray[routeNumber++] = route;

    // hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 2)
        ->withTargetAddresses(1, new byte[1]{6});
    routesArray[routeNumber++] = route;

    route = (new Switch())
            ->withSwitchAddress(relaysCount + 23)
            // ->withSwitchType(SwitchType::Push)
            ->withSwitchType(SwitchType::Toggle)
            ->withTargetAddresses(24, new byte[24]{
                0, 1, 2, 3, 4, 5, 6, 7,
                8, 9, 10, 11, 12, 13, 14, 15,
                16, 17, 18, 19, 20, 21, 22, 23,
            });
    routesArray[routeNumber++] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}