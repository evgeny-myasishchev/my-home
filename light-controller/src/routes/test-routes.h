#include <switch.h>

using namespace v2;

#define RELAY_BOARDS 2
#define INPUT_BOARDS 2

ArrayPtr<Switch *> createRoutes()
{
    const byte switchesOffset = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;

    Switch **routesArray = new Switch *[switchesCount];

    for (byte switchNr = 0; switchNr < switchesCount - 1; switchNr++)
    {
        Switch *route = (new Switch())
                            ->withSwitchAddress(switchesOffset + switchNr)
                            ->withTargetAddresses(1, new byte[1]{switchNr});
        routesArray[switchNr] = route;
    }

    routesArray[switchesCount - 1] =
        (new Switch())
            ->withSwitchType(SwitchType::Toggle)
            ->withSwitchAddress(switchesOffset + switchesCount - 1)
            ->withTargetAddresses(16, new byte[16]{
                0, 1, 2, 3, 4, 5, 6, 7,
                8, 9, 10, 11, 12, 13, 14, 15
            });

    return ArrayPtr<Switch *>(switchesCount, routesArray);
}