#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/CleanerController.h"
#include "Class/Tick.h"


#include <gtest/gtest.h>

class FakeSensor : public ISensor {
public:
    bool value = true;
    bool detect() override {
        return value;
    }
};

//Tick의 Subscribe확인
TEST(TickTest, TickSubscribeCheck) {
    EventBus bus;
    FakeSensor frontSensor;
    FakeSensor leftSensor;
    FakeSensor rightSensor;
    FakeSensor dustSensor;
    SensorController sensor(&bus, &frontSensor, &leftSensor, &rightSensor, &dustSensor);
    Tick testTick(&bus, &sensor);
    SensorController* testSensor = NULL;

    EXPECT_EQ(testTick.getDoCheck(), false);
    bus.publishMoveForward();
    EXPECT_EQ(testTick.getDoCheck(), true);
    bus.publishAvoidObstacle(testSensor);
    EXPECT_EQ(testTick.getDoCheck(), false);
    bus.publishMoveForward();
    EXPECT_EQ(testTick.getDoCheck(), true);
}

//Tick의 DoDustCheck시 doCheck 변화 확인
TEST(TickTest, WhenDoDustCheckChangeDoCheck) {
    EventBus bus;
    FakeSensor frontSensor;
    FakeSensor leftSensor;
    FakeSensor rightSensor;
    FakeSensor dustSensor;
    SensorController sensor(&bus, &frontSensor, &leftSensor, &rightSensor, &dustSensor);
    Tick testTick(&bus, &sensor);

    EXPECT_EQ(testTick.getDoCheck(), false);
    testTick.DoDustCheck();
    EXPECT_EQ(testTick.getDoCheck(), true);
}

//Tick의 stopDustCheck시 doCheck 변화 확인
TEST(TickTest, WhenStopDustCheckChangeDoCheck) {
    EventBus bus;
    FakeSensor frontSensor;
    FakeSensor leftSensor;
    FakeSensor rightSensor;
    FakeSensor dustSensor;
    SensorController sensor(&bus, &frontSensor, &leftSensor, &rightSensor, &dustSensor);
    Tick testTick(&bus, &sensor);

    EXPECT_EQ(testTick.getDoCheck(), false);
    testTick.DoDustCheck();
    EXPECT_EQ(testTick.getDoCheck(), true);
    for (int i = 0; i < 3; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        EXPECT_EQ(testTick.getDoCheck(), true);
    }   //여러번 반복 체크
    testTick.StopDustCheck();
    EXPECT_EQ(testTick.getDoCheck(), false);
}
