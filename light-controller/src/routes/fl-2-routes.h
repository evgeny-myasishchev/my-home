#include <switch.h>

// Routes for the second flour

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 3

ArrayPtr<Switch *> createRoutes()
{
    const byte switchesOffset = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch * *routesArray = new Switch*[switchesCount];

    // First two switch boards are controlling corresponding pins
    // for now
    for (byte switchNr = 0; switchNr < 16; switchNr++)
    {
        Switch* route = (new Switch())
            ->withSwitchAddress(switchesOffset + switchNr)
            ->withTargetAddresses(1, new byte[1]{switchNr});
        routesArray[switchNr] = route;
    }

    // Last board is controlling all
    byte* allRelays = new byte[RELAY_BOARDS * 8];
    for (byte relayNum = 0; relayNum < RELAY_BOARDS * 8; relayNum++)
    {
        allRelays[relayNum] = relayNum;
    }

    routesArray[16] = (new Switch())
        ->withSwitchAddress(switchesOffset + 16)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[17] = (new Switch())
        ->withSwitchAddress(switchesOffset + 17)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[18] = (new Switch())
        ->withSwitchAddress(switchesOffset + 18)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[19] = (new Switch())
        ->withSwitchAddress(switchesOffset + 19)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[20] = (new Switch())
        ->withSwitchAddress(switchesOffset + 20)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[21] = (new Switch())
        ->withSwitchAddress(switchesOffset + 21)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[22] = (new Switch())
        ->withSwitchAddress(switchesOffset + 22)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[23] = (new Switch())
        ->withSwitchAddress(switchesOffset + 23)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    routesArray[23] = (new Switch())
        ->withSwitchAddress(switchesOffset + 23)
        ->withTargetAddresses(RELAY_BOARDS, allRelays);

    return ArrayPtr<Switch *>(switchesCount, routesArray);
}