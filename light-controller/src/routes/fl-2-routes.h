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

    // Kids bedroom (green) - room light
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 0)
        ->withTargetAddresses(1, new byte[1]{5});
    routesArray[routeNumber++] = route;

    // Kids bedroom (green) - hall
    route = (new Switch())
        ->withSwitchAddress(relaysCount + 1)
        ->withTargetAddresses(1, new byte[1]{0});
    routesArray[routeNumber++] = route;

    byte* allRelays = new byte[relaysCount];
    for (byte relayNum = 0; relayNum < relaysCount; relayNum++)
    {
        allRelays[relayNum] = relayNum;
    }
    routesArray[routeNumber++] = (new Switch())
        ->withSwitchType(SwitchType::Toggle)
        ->withSwitchAddress(relaysCount + 16)
        ->withTargetAddresses(relaysCount, allRelays);

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}