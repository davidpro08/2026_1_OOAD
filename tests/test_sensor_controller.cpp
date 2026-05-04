#include <gtest/gtest.h>
#include "SensorController.h"



class FakeSensor : public ISensor {
public:
    bool detect() override {
        return true; // 항상 감지된다고 가정
    }
    bool detect(bool value) {
        return value; 
    }
};

class SensorControllerTest  : public ::testing::Test {
protected:
    EventBus bus;
    FakeSensor frontSensor;
    FakeSensor leftSensor;
    FakeSensor rightSensor;
    FakeSensor dustSensor;
    SensorController sensorController;

    SensorControllerTest() : sensorController(&bus, &frontSensor, &leftSensor, &rightSensor, &dustSensor) {}
};

TEST_F(SensorControllerTest, TestTurnOn) {
    sensorController.turnOn();//실행

    EXPECT_TRUE(sensorController.getIsTurnOn() == true);//결과
}

TEST_F(SensorControllerTest, TestTurnOff) {
    sensorController.turnOn();//초기화

    sensorController.turnOff();//실행

    EXPECT_TRUE(sensorController.getIsTurnOn() == false);//결과
}

TEST_F(SensorControllerTest, TestFrontObstacleDetected) {
    bool isAvoidObstacleCalled = false;

    bus.subScribeAvoidObstacle([&isAvoidObstacleCalled](SensorController* sender) {
        isAvoidObstacleCalled = true;
    });

    sensorController.FrontObstacleDetected();

    EXPECT_TRUE(isAvoidObstacleCalled);
}

TEST_F(SensorControllerTest, TestChecknPowerUp) {
    bool isDetectedDustCalled = false;

    bus.subScribeDetectedDust([&isDetectedDustCalled]() {
        isDetectedDustCalled = true;
    });

    sensorController.ChecknPowerUp();

    EXPECT_TRUE(isDetectedDustCalled);
}

TEST_F(SensorControllerTest, TestGetLeftState) {
    EXPECT_TRUE(sensorController.getLeftState());
}// 항상 감지된다고 가정한 FakeSensor를 사용하여 getLeftState가 true를 반환하는지 테스트로 충분한가

TEST_F(SensorControllerTest, TestGetRightState) {
    EXPECT_TRUE(sensorController.getRightState());
}// 항상 감지된다고 가정한 FakeSensor를 사용하여 getRightState가 true를 반환하는지 테스트로 충분한가