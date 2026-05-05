#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/CleanerController.h"
#include "Class/Timer.h"
#include "Class/HwCleaner.h"

#include <gtest/gtest.h>

class FakeHwCleaner : public HwCleaner {
public:
    bool isturnOn = false;
    bool ispowerUp = true;
public:
    void powerUp() override {
        ispowerUp = true;
    }
    void powerRestore() override {
        ispowerUp = false;
    }
    void turnOn() override {
        isturnOn = true;
    }
    void turnOff() override {
        isturnOn = false;
    }
};

//CleanerController를 상속받은 testClass 생성(private까지 확인하기 위해)
class TestCleaner : public CleanerController {
public:
    using CleanerController::CleanerController; // 생성자 상속

    // private(protected) 변수 값을 반환하는 public 메서드 추가
    Timer& GetTimer() { return timer; }
};

//TurnOn,Off
TEST(CleanerTest, TurnOnTurnOffDoesNotThrow) {
    EventBus bus;
    FakeHwCleaner testHwCleaner;
    TestCleaner myTestCleaner(&bus, &testHwCleaner);
    myTestCleaner.turnOn();
    EXPECT_EQ(testHwCleaner.isturnOn, true);
    myTestCleaner.turnOff();
    EXPECT_EQ(testHwCleaner.isturnOn, false);
}

//PowerUp bool Type 체크
TEST(CleanerTest, PowerUpandRestoreCheck) {
    EventBus bus;
    FakeHwCleaner testHwCleaner;
    TestCleaner myTestCleaner(&bus, &testHwCleaner);
    myTestCleaner.CCpowerUp();
    EXPECT_EQ(testHwCleaner.ispowerUp, true);
    myTestCleaner.CCpowerRestore();
    EXPECT_EQ(testHwCleaner.ispowerUp, false);
}

//Subscribe 체크
TEST(CleanerTest, CleanerControllerBusSubscribeCheck) {
    EventBus bus;
    FakeHwCleaner testHwCleaner;
    TestCleaner myTestCleaner(&bus, &testHwCleaner);
    SensorController* testSensor = NULL;
    bus.publishMoveForward();
    EXPECT_EQ(testHwCleaner.isturnOn, true);
    bus.publishDetectedDust();
    EXPECT_EQ(testHwCleaner.ispowerUp, true);
    bus.publishAvoidObstacle(testSensor);
    EXPECT_EQ(testHwCleaner.isturnOn, false);
    bus.publishMoveForward();
    EXPECT_EQ(testHwCleaner.isturnOn, true);
    bus.publishTurnOff();
    EXPECT_EQ(testHwCleaner.isturnOn, false);
}

//PowerUp시 타이머 호출 Check
TEST(CleanerTest, WhenPowerUpTimerCallingCheck) {
    EventBus bus;
    FakeHwCleaner testHwCleaner;
    TestCleaner myTestCleaner(&bus, &testHwCleaner);
    auto& t = myTestCleaner.GetTimer();
    EXPECT_EQ(t.getCurrent_Time(), 0);
    myTestCleaner.CCpowerUp();
    EXPECT_GT(t.getCurrent_Time(), 0);
}

//Hw 연결
TEST(CleanerTest, CleanerHwConnectCheck) {
    EventBus bus;
    FakeHwCleaner testHwCleaner;
    TestCleaner myTestCleaner(&bus, &testHwCleaner);
    auto& t = myTestCleaner.GetTimer();

    myTestCleaner.turnOn();
    EXPECT_EQ(testHwCleaner.isturnOn, true);
    myTestCleaner.turnOff();
    EXPECT_EQ(testHwCleaner.isturnOn, false);
    myTestCleaner.CCpowerUp();
    EXPECT_EQ(testHwCleaner.ispowerUp, true);
    myTestCleaner.CCpowerRestore();
    EXPECT_EQ(testHwCleaner.ispowerUp, false);
}

//PowerUp시간 종료시 Restore Check
TEST(CleanerTest, PowerRestoreCheck) {
    EventBus bus;
    FakeHwCleaner testHwCleaner;
    TestCleaner myTestCleaner(&bus, &testHwCleaner);
    myTestCleaner.CCpowerUp();
    EXPECT_EQ(testHwCleaner.ispowerUp, true);
    std::this_thread::sleep_for(std::chrono::seconds(3));   //3초까지는 powerup
    EXPECT_EQ(testHwCleaner.ispowerUp, true);
    std::this_thread::sleep_for(std::chrono::seconds(3));   //6초에는 powerRestore
    EXPECT_EQ(testHwCleaner.ispowerUp, false);
}

