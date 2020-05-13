#include <switch.h>

// Routes for the second flour

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 3

ArrayPtr<Switch *> createRoutes()
{
    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch * *routesArray = new Switch*[switchesCount];

    // First two switch boards are controlling corresponding pins
    // for now
    // for (byte switchNr = 0; switchNr < 15; switchNr++)
    // {
    //     Switch* route = (new Switch())
    //         ->withSwitchAddress(relaysCount + switchNr)
    //         ->withTargetAddresses(1, new byte[1]{switchNr});
    //     routesArray[switchNr] = route;
    // }

    byte routeNumber = 0;
    Switch* route;

    // =================== Kids bedroom (green) ===================
    // room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 0)
        ->withTargetAddresses(1, new byte[1]{5});
    routesArray[routeNumber++] = route;

    // hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 1)
        ->withTargetAddresses(2, new byte[2]{0, 13});
    routesArray[routeNumber++] = route;

    // =================== Kids bedroom (blue) ===================
    // room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 2)
        ->withTargetAddresses(1, new byte[1]{4});
    routesArray[routeNumber++] = route;

    // hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 3)
        ->withTargetAddresses(2, new byte[2]{0, 13});
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

    // balcony
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 15)
        ->withTargetAddresses(1, new byte[1]{2});
    routesArray[routeNumber++] = route;

    // byte* allRelays = new byte[relaysCount];
    // for (byte relayNum = 0; relayNum < relaysCount; relayNum++)
    // {
    //     allRelays[relayNum] = relayNum;
    // }
    // routesArray[routeNumber++] = (new Switch())
    //     ->withSwitchType(SwitchType::Toggle)
    //     ->withSwitchAddress(relaysCount + 16)
    //     ->withTargetAddresses(relaysCount, allRelays);

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}