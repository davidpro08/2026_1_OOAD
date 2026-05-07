#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/CleanerController.h"
#include "Class/Timer.h"
#include "HDWARE/HwCleaner.h"

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

//CleanerController�� ��ӹ��� testClass ����(private���� Ȯ���ϱ� ����)
class TestCleaner : public CleanerController {
public:
    using CleanerController::CleanerController; // ������ ���

    // private(protected) ���� ���� ��ȯ�ϴ� public �޼��� �߰�
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

//PowerUp bool Type üũ
//TEST(CleanerTest, PowerUpandRestoreCheck) {
//    //EventBus bus;
//    //FakeHwCleaner testHwCleaner;
//    //TestCleaner myTestCleaner(&bus, &testHwCleaner);
//    //myTestCleaner.CCpowerUp();
//    //EXPECT_EQ(testHwCleaner.ispowerUp, true);
//    //myTestCleaner.CCpowerRestore(); 
//    //EXPECT_EQ(testHwCleaner.ispowerUp, false);
//}

//Subscribe üũ
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

//PowerUp�� Ÿ�̸� ȣ�� Check
TEST(CleanerTest, WhenPowerUpTimerCallingCheck) {
    
}

//Hw ����
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

////PowerUp�ð� ����� Restore Check
//TEST(CleanerTest, PowerRestoreCheck) {
//    EventBus bus;
//    FakeHwCleaner testHwCleaner;
//    TestCleaner myTestCleaner(&bus, &testHwCleaner);
//    myTestCleaner.CCpowerUp();
//    EXPECT_EQ(testHwCleaner.ispowerUp, true);
//    std::this_thread::sleep_for(std::chrono::seconds(3));   //3�ʱ����� powerup
//    EXPECT_EQ(testHwCleaner.ispowerUp, true);
//    std::this_thread::sleep_for(std::chrono::seconds(3));   //6�ʿ��� powerRestore
//    EXPECT_EQ(testHwCleaner.ispowerUp, false);
//}

