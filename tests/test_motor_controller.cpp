#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"

#include <gtest/gtest.h>

namespace {

class FakeSensorProvider : public SensorProvider {
public:
    bool left = true;
    bool right = true;

    bool getLeftState() override { return left; }
    bool getRightState() override { return right; }
};

class FakeSensor : public ISensor {
public:
    bool detect() override {
        return true; // 항상 감지된다고 가정
    }
    bool detect(bool value) {
        return value; 
    }
};

}  // namespace

TEST(MotorController, TurnOnTurnOffDoesNotThrow) {
    EventBus bus;
    MotorController motor(&bus);
    motor.turnOn();
    motor.turnOff();
}

TEST(MotorController, PublishMoveForwardNotifiesSubscribers) {
    EventBus bus;
    MotorController motor(&bus);
    int move_forward_count = 0;
    bus.subScribeMoveForward([&move_forward_count] { ++move_forward_count; });

    bus.publishMoveForward();

    EXPECT_EQ(move_forward_count, 1);
}

TEST(MotorController, AvoidObstacleWhenLeftBlockedPublishesMoveForward) {
    EventBus bus;
    MotorController motor(&bus);
    int move_forward_count = 0;
    bus.subScribeMoveForward([&move_forward_count] { ++move_forward_count; });

    FakeSensorProvider fake;
    fake.left = false;
    fake.right = true;
    motor.AvoidObstacle(fake);

    EXPECT_EQ(move_forward_count, 1);
}

TEST(MotorController, AvoidObstacleWhenRightBlockedPublishesMoveForward) {
    EventBus bus;
    MotorController motor(&bus);
    int move_forward_count = 0;
    bus.subScribeMoveForward([&move_forward_count] { ++move_forward_count; });

    FakeSensorProvider fake;
    fake.left = true;
    fake.right = false;
    motor.AvoidObstacle(fake);

    EXPECT_EQ(move_forward_count, 1);
}

TEST(MotorController, AvoidObstacleWhenBothSidesBlockedStillPublishesMoveForward) {
    EventBus bus;
    MotorController motor(&bus);
    int move_forward_count = 0;
    bus.subScribeMoveForward([&move_forward_count] { ++move_forward_count; });

    FakeSensorProvider fake;
    fake.left = true;
    fake.right = true;
    motor.AvoidObstacle(fake);

    EXPECT_EQ(move_forward_count, 1);
}

TEST(MotorController, AvoidObstacleViaEventBusUsesSensorControllerState) {
    EventBus bus;
    MotorController motor(&bus);
    int move_forward_count = 0;
    bus.subScribeMoveForward([&move_forward_count] { ++move_forward_count; });
    FakeSensor frontSensor;
    FakeSensor leftSensor;
    FakeSensor rightSensor;
    FakeSensor dustSensor;

    SensorController sensor(&bus,&frontSensor,&leftSensor,&rightSensor,&dustSensor);
    bus.publishAvoidObstacle(&sensor);

    EXPECT_EQ(move_forward_count, 1);
}
