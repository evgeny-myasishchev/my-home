#include <switch.h>
#include <solar-switch.h>

using namespace v2;

#define RELAY_BOARDS 3
#define INPUT_BOARDS 3

#define SPOT_ADDR_HALL_ENTRY 0
#define SPOT_ADDR_MAIN_ENTRY 1
#define SPOT_ADDR_OFFICE_COMMON 2
#define SPOT_ADDR_BOILER 3
#define SPOT_ADDR_ENTRY_BACK1 4
#define SPOT_ADDR_ENTRY_BAC2 5
#define SPOT_ADDR_HALL_COMMON 6
#define SPOT_ADDR_HALL_NIGHT 7
#define SPOT_ADDR_STAIRS_NIGHT 8
#define SPOT_ADDR_UNDER_STAIRS 9
#define SPOT_ADDR_BATH_COMMON 10
#define SPOT_ADDR_WARDEROBE 11
#define SPOT_ADDR_KITCHEN_COMMON 12
#define SPOT_ADDR_KITCHEN_STORE 13
#define SPOT_ADDR_KITCHEN_LED 14
#define SPOT_ADDR_LIVING_COMM 15
#define SPOT_ADDR_LIVING_TABLE 16
#define SPOT_ADDR_LIVING_SIDE1 17
#define SPOT_ADDR_LIVING_SIDE2 18
#define SPOT_ADDR_MAIN_ENTRY_NIGHT 19

ArrayPtr<Switch *> createRoutes(SolarSwitch *solarSwitch)
{
    const byte relaysCount = RELAY_BOARDS * 8;
    const byte switchesCount = INPUT_BOARDS * 8;
    const byte actualSwitchesCount = 16; // to save mem

    Switch **routesArray = new Switch *[actualSwitchesCount];
    byte routeNumber = 0;
    Switch *route;

    // virtual
    const auto virtualAddressStart = relaysCount + switchesCount;
    solarSwitch->setPin(virtualAddressStart, 0, 1);
    solarSwitch->setSunriseOffsetMinutes(0);
    solarSwitch->setSunriseOffsetMinutes(0);

    // =================== Office ===================
    // room light
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 0)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_OFFICE_COMMON});
    routesArray[routeNumber] = route;

    // under stairs
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 1)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_UNDER_STAIRS});
    routesArray[++routeNumber] = route;

    // hall
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 2)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_HALL_COMMON});
    routesArray[++routeNumber] = route;

    // =================== Living ===================
    // kitchen store
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 3)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_KITCHEN_STORE});
    routesArray[++routeNumber] = route;

    // living common
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 4)
                ->withPrimaryTargetAddress(SPOT_ADDR_LIVING_COMM)
                ->withTargetAddresses(3, new byte[3]{
                                             SPOT_ADDR_LIVING_COMM,
                                             SPOT_ADDR_LIVING_SIDE1,
                                             SPOT_ADDR_LIVING_SIDE2});
    routesArray[++routeNumber] = route;

    // living side
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 5)
                ->withTargetAddresses(2, new byte[2]{SPOT_ADDR_LIVING_SIDE1, SPOT_ADDR_LIVING_SIDE2});
    routesArray[++routeNumber] = route;

    // hall entry
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 6)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_HALL_ENTRY});
    routesArray[++routeNumber] = route;

    // =================== kitchen ===================
    // living table
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 7)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_LIVING_TABLE});
    routesArray[++routeNumber] = route;

    // kitchen led
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 8)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_KITCHEN_LED});
    routesArray[++routeNumber] = route;

    // kitchen common
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 9)
                ->withPrimaryTargetAddress(SPOT_ADDR_KITCHEN_LED)
                ->withTargetAddresses(2, new byte[2]{SPOT_ADDR_KITCHEN_COMMON,
                                                     SPOT_ADDR_KITCHEN_LED});
    routesArray[++routeNumber] = route;

    // =================== hall ===================
    // common
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 10)
                ->withTargetAddresses(2, new byte[2]{SPOT_ADDR_HALL_COMMON, SPOT_ADDR_HALL_NIGHT});
    routesArray[++routeNumber] = route;

    // store
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 11)
                ->withTargetAddresses(2, new byte[2]{SPOT_ADDR_BOILER, SPOT_ADDR_WARDEROBE});
    routesArray[++routeNumber] = route;

    // =================== bath ===================
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 12)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_BATH_COMMON});
    routesArray[++routeNumber] = route;

    // =================== Entry ===================
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 22)
                ->withPrimaryTargetAddress(SPOT_ADDR_HALL_ENTRY)
                ->withTargetAddresses(3, new byte[3]{
                                             SPOT_ADDR_HALL_ENTRY,
                                             SPOT_ADDR_HALL_COMMON,
                                             SPOT_ADDR_HALL_NIGHT});
    routesArray[++routeNumber] = route;

    // entry all off
    route = (new Switch())
                ->withSwitchAddress(relaysCount + 23)
                ->withSwitchType(SwitchType::Push)
                ->withSwitchAction(SwitchAction::ForceOff)
                ->withTargetAddresses(20, new byte[20]{
                                              SPOT_ADDR_HALL_ENTRY,
                                              SPOT_ADDR_MAIN_ENTRY,
                                              SPOT_ADDR_OFFICE_COMMON,
                                              SPOT_ADDR_BOILER,
                                              SPOT_ADDR_ENTRY_BACK1,
                                              SPOT_ADDR_ENTRY_BAC2,
                                              SPOT_ADDR_HALL_COMMON,
                                              SPOT_ADDR_HALL_NIGHT,
                                              SPOT_ADDR_STAIRS_NIGHT,
                                              SPOT_ADDR_UNDER_STAIRS,
                                              SPOT_ADDR_BATH_COMMON,
                                              SPOT_ADDR_WARDEROBE,
                                              SPOT_ADDR_KITCHEN_COMMON,
                                              SPOT_ADDR_KITCHEN_STORE,
                                              SPOT_ADDR_KITCHEN_LED,
                                              SPOT_ADDR_LIVING_COMM,
                                              SPOT_ADDR_LIVING_TABLE,
                                              SPOT_ADDR_LIVING_SIDE1,
                                              SPOT_ADDR_LIVING_SIDE2,
                                              SPOT_ADDR_MAIN_ENTRY_NIGHT,
                                          });
    routesArray[++routeNumber] = route;

    // entry solar
    route = (new Switch())
                ->withSwitchAddress(virtualAddressStart + 0)
                ->withSwitchType(SwitchType::Toggle)
                ->withTargetAddresses(1, new byte[1]{SPOT_ADDR_MAIN_ENTRY_NIGHT});
    routesArray[++routeNumber] = route;

    return ArrayPtr<Switch *>(routeNumber, routesArray);
}