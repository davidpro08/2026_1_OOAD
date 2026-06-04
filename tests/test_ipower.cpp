#include "Class/EventBus.h"
#include "Class/IPower.h"
#include "Class/MotorController.h"
#include "HDWARE/Motor.h"

#include <gtest/gtest.h>

class FakePower : public IPower {
public:
    bool didTurnOff = false;

    void turnOff() override {
        didTurnOff = true;
    }
};

class FakeMotor : public Motor {
public:
    bool stopped = false;
    bool movedForward = false;
    bool movedBackward = false;
    bool turnedLeft = false;
    bool turnedRight = false;

    void stop() override {
        stopped = true;
    }
    void moveForward() override {
        movedForward = true;
    }
    void moveBackward() override {
        movedBackward = true;
    }
    void turnLeft() override {
        turnedLeft = true;
    }
    void turnRight() override {
        turnedRight = true;
    }
};

TEST(IPowerTest, DerivedTurnOffOverrideIsCalled) {
    FakePower fake;
    IPower* power = &fake;

    power->turnOff();

    EXPECT_TRUE(fake.didTurnOff);
}

TEST(IPowerTest, MotorControllerCanBeUsedThroughIPowerPointer) {
    EventBus bus;
    FakeMotor motor;
    MotorController controller(&bus, motor);

    bool turnOffPublished = false;
    bus.subScribeTurnOff([&]() {
        turnOffPublished = true;
    });

    IPower* power = &controller;
    power->turnOff();

    EXPECT_FALSE(turnOffPublished);
}
