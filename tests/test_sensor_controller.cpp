#include <gtest/gtest.h>
#include "Class/SensorController.h"
#include "Class/ISensor.h"
#include "Class/EventBus.h"



class FakeSensor : public ISensor {
public:
    bool value = true;
    bool detect() override{
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

TEST_F(SensorControllerTest, TestFrontObstacleDetectedTrue) {
    bool isAvoidObstacleCalled = false;

    bus.subScribeAvoidObstacle([&isAvoidObstacleCalled](SensorController* sender) {
        isAvoidObstacleCalled = true;
    });

    sensorController.FrontObstacleDetected();

    EXPECT_TRUE(isAvoidObstacleCalled);
}// 이거는 ISensor가 아니니까 다르게 해야하나

TEST_F(SensorControllerTest, TestChecknPowerUpTrue) {
	dustSensor.value = true; // 먼지 감지 상태로 설정

    bool isDetectedDustCalled = false;

    bus.subScribeDetectedDust([&isDetectedDustCalled]() {
        isDetectedDustCalled = true;
    });

    sensorController.ChecknPowerUp();

    EXPECT_TRUE(isDetectedDustCalled);
}

TEST_F(SensorControllerTest, TestChecknPowerUp_WhenNoDust) {
    dustSensor.value = false;// 먼지 비감지 상태로 설정

    bool isDetectedDustCalled = false;

    bus.subScribeDetectedDust([&isDetectedDustCalled]() {
        isDetectedDustCalled = true;
        });

    sensorController.ChecknPowerUp();

    EXPECT_FALSE(isDetectedDustCalled);
}

TEST_F(SensorControllerTest, TestGetLeftStateTrue) {
    leftSensor.value = true;

    EXPECT_TRUE(sensorController.getLeftState());
}

TEST_F(SensorControllerTest, TestGetLeftStateFalse) {
        leftSensor.value = false;

        EXPECT_FALSE(sensorController.getLeftState());
}

TEST_F(SensorControllerTest, TestGetRightStateFalse) {
	  rightSensor.value = false;
      EXPECT_FALSE(sensorController.getRightState());
}

TEST_F(SensorControllerTest, TestGetRightStateTrue) {
      rightSensor.value = true; 
      EXPECT_TRUE(sensorController.getRightState());
}