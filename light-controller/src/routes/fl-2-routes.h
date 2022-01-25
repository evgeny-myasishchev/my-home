#include <switch.h>

// Routes for the second flour

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 3

#define SPOT_ADDR_BALCONY 2
#define SPOT_ADDR_WARDEROBE 9

ArrayPtr<Switch *> createRoutes(SolarSwitch *solarSwitch)
{
    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch **routesArray = new Switch*[switchesCount];
    byte routeNumber = 0;
    Switch* route;

    // virtual
    const auto virtualAddressStart = relaysCount + switchesCount;
    solarSwitch->setPin(virtualAddressStart, 0, 1);
    solarSwitch->setSunriseOffsetMinutes(0);
    solarSwitch->setSunriseOffsetMinutes(0);

    // =================== Kids bedroom (green) ===================
    // hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 0)
        ->withTargetAddresses(2, new byte[2]{0, 13});
    routesArray[routeNumber++] = route;

    // room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 1)
        ->withTargetAddresses(1, new byte[1]{5});
    routesArray[routeNumber++] = route;

    // =================== Kids bedroom (blue) ===================
    // hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 2)
        ->withTargetAddresses(2, new byte[2]{0, 13});
    routesArray[routeNumber++] = route;

    // room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 3)
        ->withTargetAddresses(1, new byte[1]{4});
    routesArray[routeNumber++] = route;

    // =================== Parents bedroom ===================
    // entry: room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 4)
        ->withTargetAddresses(1, new byte[1]{6});
    routesArray[routeNumber++] = route;

    // entry: hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 5)
        ->withTargetAddresses(2, new byte[2]{0, 13});
    routesArray[routeNumber++] = route;

    // bed left
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 6)
        ->withTargetAddresses(1, new byte[1]{8});
    routesArray[routeNumber++] = route;

    // bed right
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 7)
        ->withTargetAddresses(1, new byte[1]{7});
    routesArray[routeNumber++] = route;

    // beds: room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 8)
        ->withTargetAddresses(1, new byte[1]{6});
    routesArray[routeNumber++] = route;

    // beds: hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 9)
        ->withTargetAddresses(2, new byte[2]{0, 13});
    routesArray[routeNumber++] = route;

    // =================== Bath ===================
    // room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 10)
        ->withTargetAddresses(1, new byte[1]{10});
    routesArray[routeNumber++] = route;

    // mirror
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 11)
        ->withTargetAddresses(1, new byte[1]{11});
    routesArray[routeNumber++] = route;

    // laundry
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 12)
        ->withTargetAddresses(1, new byte[1]{12});
    routesArray[routeNumber++] = route;

    // =================== Hall ===================
    // hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 14)
        ->withTargetAddresses(2, new byte[2]{0, 13});
    routesArray[routeNumber++] = route;

    // balcony (for now used for store)
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 15)
        ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_BALCONY}); 
    routesArray[routeNumber++] = route;

    // =================== Warderobe ===================
    // warderobe
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 16)
        ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_WARDEROBE});
    routesArray[routeNumber++] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}