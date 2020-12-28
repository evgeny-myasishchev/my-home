#include <switch.h>

using namespace v2;

#define RELAY_BOARDS 3
#define INPUT_BOARDS 3

#define SPOT_ADDR_HALL_COMMON 6
#define SPOT_ADDR_KITCHEN_STORE 13
#define SPOT_ADDR_MAIN_ENTRY 19

ArrayPtr<Switch *> createRoutes(SolarSwitch *solarSwitch)
{
    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;
    const byte actualSwitchesCount = 15; // to save mem

    Switch **routesArray = new Switch *[actualSwitchesCount];
    byte routeNumber = 0;
    Switch* route;

    // virtual
    const auto virtualAddressStart = relaysCount + switchesCount;
    solarSwitch->setPin(virtualAddressStart, 0, 1);
    solarSwitch->setSunriseOffsetMinutes(0);
    solarSwitch->setSunriseOffsetMinutes(0);

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
        ->withTargetAddresses(3, new byte[3]{
            0, 1, SPOT_ADDR_HALL_COMMON
        });
    routesArray[routeNumber++] = route;

    // =================== Living ===================
    // kitchen store
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 3)
        ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_KITCHEN_STORE});
    routesArray[routeNumber++] = route;

    // living common
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 4)
        ->withTargetAddresses(4, new byte[4]{15, 16, 17, 18});
    routesArray[routeNumber++] = route;

    // living side
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 5)
        ->withTargetAddresses(1, new byte[1]{14});
    routesArray[routeNumber++] = route;

    // hall entry
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 6)
        ->withTargetAddresses(3, new byte[3]{
            0, 1, SPOT_ADDR_HALL_COMMON
        });
    routesArray[routeNumber++] = route;

    // =================== kitchen ===================
    // living table
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 7)
        ->withTargetAddresses(1, new byte[1]{16});
    routesArray[routeNumber++] = route;

    // kitchen led (store for now)
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 8)
        ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_KITCHEN_STORE});
    routesArray[routeNumber++] = route;

    // kitchen common
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 9)
        ->withTargetAddresses(2, new byte[2]{12, 14});
    routesArray[routeNumber++] = route;

    // =================== hall ===================
    // common
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 10)
        ->withTargetAddresses(3, new byte[3]{0, 1, 6});
    routesArray[routeNumber++] = route;

    // store
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 11)
        ->withTargetAddresses(2, new byte[2]{3, 11});
    routesArray[routeNumber++] = route;

    // =================== bath ===================
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 12)
        ->withTargetAddresses(1, new byte[1]{10});
    routesArray[routeNumber++] = route;

    // =================== Entry ===================
    route = (new Switch())
            ->withSwitchAddress(relaysCount + 22)
            ->withTargetAddresses(3, new byte[3]{
                0, 1, 6
            });
    routesArray[routeNumber++] = route;

    // entry all
    // route = (new Switch())
    //         ->withSwitchAddress(relaysCount + 23)
    //         // ->withSwitchType(SwitchType::Push)
    //         ->withSwitchType(SwitchType::Toggle)
    //         ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_MAIN_ENTRY});
    //         // ->withTargetAddresses(24, new byte[24]{
    //         //     0, 1, 2, 3, 4, 5, 6, 7,
    //         //     8, 9, 10, 11, 12, 13, 14, 15,
    //         //     16, 17, 18, 19, 20, 21, 22, 23,
    //         // });

    // entry solar
    route = (new Switch())
        ->withSwitchAddress(virtualAddressStart + 0)
        ->withSwitchType(SwitchType::Toggle)
        ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_MAIN_ENTRY});
    routesArray[routeNumber++] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}