#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <arduino-compat.h>
#include <stdio.h>
#include "test-lib/random.h"
#include <switches-router-v2.h>
#include "test-lib/test-pin-bus.h"

namespace
{

using namespace v2;
using namespace testing;

class MockSwitchService : public SwitchService
{
public:
    MockSwitchService() : SwitchService(SwitchServiceConfig{}) {}
    MOCK_METHOD(void, processSignal, (byte signal, Switch *sw));
    MOCK_METHOD(byte, getTargetState, (byte currentValue, Switch *sw));
    MOCK_METHOD(void, applyStateChange, (Switch * sw));
};

class V2SwitchesRouterTest : public ::testing::Test
{
public:
    TestPinBus *bus;
    SwitchesRouter *router;
    testing::NiceMock<MockSwitchService> *pushBtnSwitchSvc;
    testing::NiceMock<MockSwitchService> *toggleBtnSwitchSvc;

protected:
    void SetUp() override
    {
        bus = new TestPinBus(2);
        bus->pendingTestState = new byte[2]();
        pushBtnSwitchSvc = new testing::NiceMock<MockSwitchService>();
        toggleBtnSwitchSvc = new testing::NiceMock<MockSwitchService>();
        SwitchRouterServices services{
            .bus = bus,
            .pushBtnSwitchSvc = pushBtnSwitchSvc,
            .toggleBtnSwitchSvc = toggleBtnSwitchSvc,
        };
        router = new SwitchesRouter(services);
    }
    void TearDown() override
    {
        delete bus->pendingTestState;
        delete bus;
        delete router;
        delete toggleBtnSwitchSvc;
        delete pushBtnSwitchSvc;
    }
};

TEST_F(V2SwitchesRouterTest, processRoutesNoChanges)
{
    const byte state = test::randomNumber(0, 255);
    bus->pendingTestState[0] = state;
    bus->readState();

    Switch route1{.switchAddress = 0};
    Switch route2{.switchAddress = 1};
    Switch route3{.switchAddress = 2, .type = SwitchType::Toggle};
    Switch route4{.switchAddress = 3};
    Switch route5{.switchAddress = 4, .type = SwitchType::Toggle};
    Switch *routesArray[] = {&route4, &route2, &route3, &route1, &route5};
    ArrayPtr<Switch *> routes(5, (Switch **)&routesArray);

    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route1.switchAddress), &route1));
    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route2.switchAddress), &route2));
    EXPECT_CALL((*toggleBtnSwitchSvc), processSignal(bitRead(state, route3.switchAddress), &route3));
    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route4.switchAddress), &route4));
    EXPECT_CALL((*toggleBtnSwitchSvc), processSignal(bitRead(state, route5.switchAddress), &route5));

    router->processRoutes(routes);
}

void markSwitchChanged(byte signal, Switch *sw)
{
    sw->state = signal;
    sw->stateChanged = true;
}

TEST_F(V2SwitchesRouterTest, processRoutesWithChanges)
{
    const byte state = 0b00000101;
    const byte targetState = 0b00000000;
    bus->pendingTestState[0] = state;
    bus->pendingTestState[1] = targetState;
    bus->readState();

    byte route1Targets[] = {8, 9};
    Switch route1{.switchAddress = 0,
                  .targetAddresses = ArrayPtr<byte>(2, (byte *)&route1Targets)};
    Switch route2{.switchAddress = 1};
    byte route3Targets[] = {12, 13};
    Switch route3{
        .switchAddress = 2,
        .targetAddresses = ArrayPtr<byte>(2, (byte *)&route3Targets),
        .type = SwitchType::Toggle};
    Switch route4{.switchAddress = 3};
    Switch route5{.switchAddress = 4, .type = SwitchType::Toggle};
    Switch *routesArray[] = {&route1, &route4, &route2, &route3, &route5};
    ArrayPtr<Switch *> routes(5, (Switch **)&routesArray);

    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route1.switchAddress), &route1))
        .WillOnce(Invoke(markSwitchChanged));
    EXPECT_CALL((*pushBtnSwitchSvc), getTargetState(0, &route1))
        .Times(2).WillRepeatedly(Return(HIGH));
    EXPECT_CALL((*pushBtnSwitchSvc), applyStateChange(&route1));

    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route2.switchAddress), &route2));

    EXPECT_CALL((*toggleBtnSwitchSvc), processSignal(bitRead(state, route3.switchAddress), &route3))
        .WillOnce(Invoke(markSwitchChanged));
    EXPECT_CALL((*toggleBtnSwitchSvc), getTargetState(0, &route3))
        .Times(2).WillRepeatedly(Return(HIGH));
    EXPECT_CALL((*toggleBtnSwitchSvc), applyStateChange(&route3));

    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route4.switchAddress), &route4));
    EXPECT_CALL((*toggleBtnSwitchSvc), processSignal(bitRead(state, route5.switchAddress), &route5));

    router->processRoutes(routes);

    bus->writeState();

    EXPECT_EQ(bus->pendingTestState[1], 0b00110011);
}

TEST_F(V2SwitchesRouterTest, processRoutesUseActualValueOfTargets)
{
    const byte state = 0b00000111;
    const byte targetState = 0b00000000;
    bus->pendingTestState[0] = state;
    bus->pendingTestState[1] = targetState;
    bus->readState();

    byte route1Targets[] = {8, 9, 10};
    Switch route1{.switchAddress = 0,
                  .targetAddresses = ArrayPtr<byte>(3, (byte *)&route1Targets)};

    byte route2Targets[] = {8, 9, 10};
    Switch route2{
        .switchAddress = 1,
        .targetAddresses = ArrayPtr<byte>(3, (byte *)&route2Targets)};

    byte route3Targets[] = {8, 9, 10};
    Switch route3{
        .switchAddress = 2,
        .targetAddresses = ArrayPtr<byte>(3, (byte *)&route3Targets)};

    Switch *routesArray[] = {&route1, &route2, &route3};
    ArrayPtr<Switch *> routes(3, (Switch **)&routesArray);

    // route 1
    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route1.switchAddress), &route1))
        .WillOnce(Invoke(markSwitchChanged));

    // all targets are 0 initially
    EXPECT_CALL((*pushBtnSwitchSvc), getTargetState(0, &route1))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(LOW));
    EXPECT_CALL((*pushBtnSwitchSvc), applyStateChange(&route1));

    // route 2
    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route2.switchAddress), &route2))
        .WillOnce(Invoke(markSwitchChanged));

    // now target 8 is high, 9 and 10 are low
    EXPECT_CALL((*pushBtnSwitchSvc), getTargetState(HIGH, &route2))
        .WillOnce(Return(LOW));
    EXPECT_CALL((*pushBtnSwitchSvc), getTargetState(LOW, &route2))
        .Times(2)
        .WillRepeatedly(Return(HIGH));
    EXPECT_CALL((*pushBtnSwitchSvc), applyStateChange(&route2));

    // route 3
    EXPECT_CALL((*pushBtnSwitchSvc), processSignal(bitRead(state, route3.switchAddress), &route3))
        .WillOnce(Invoke(markSwitchChanged));

    // and finally 8 is low, 9 and 10 are high
    EXPECT_CALL((*pushBtnSwitchSvc), getTargetState(LOW, &route3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL((*pushBtnSwitchSvc), getTargetState(HIGH, &route3))
        .Times(2)
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL((*pushBtnSwitchSvc), applyStateChange(&route3));

    router->processRoutes(routes);

    bus->writeState();

    EXPECT_EQ(bus->pendingTestState[1], 0b00000101);
}

} // namespace
