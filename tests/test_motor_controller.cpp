#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/ISensor.h"
#include "HDWARE/Motor.h"
#include "Simulator/Point.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace MotorControllerTest {

            class FakeSensor : public ISensor {
public:
    bool value = true;
    bool detect() override{
        return value; 
    }
};

    class FakeSensorProvider : public SensorProvider {
    public:
        bool left = true;

        bool getLeftState() override { return left; }
    };

    class ScriptedSensorProvider : public SensorProvider {
    public:
        std::vector<bool> leftStates;
        size_t leftIndex = 0;

        bool getLeftState() override {
            if (leftIndex < leftStates.size()) {
                return leftStates[leftIndex++];
            }
            return leftStates.empty() ? true : leftStates.back();
        }
    };

    class FakeMotor : public Motor {
    public:
        Point point = Point(0, 0);
        Point direction = Point(0, 1);

        int stopCount = 0;
        int forwardCount = 0;
        int backwardCount = 0;
        int leftCount = 0;
        int rightCount = 0;

        void stop() override {
            stopCount++;
        }

        void moveForward() override {
            point.x = point.x + direction.x;
            point.y = point.y + direction.y;
            forwardCount++;
        }

        void turnLeft() override {
            Point temp = direction;
            direction.x = -temp.y;
            direction.y = temp.x;
            leftCount++;
        }

        void turnRight() override {
            Point temp = direction;
            direction.x = temp.y;
            direction.y = -temp.x;
            rightCount++;
        }

        void moveBackward() override {
            point.x = point.x - direction.x;
            point.y = point.y - direction.y;
            backwardCount++;
        }
    };


    class MotorControllerPowerTest : public ::testing::Test {
    protected:
        EventBus bus;
        FakeMotor motor;
        std::unique_ptr<MotorController> motorController;
        FakeSensorProvider sensorProvider;

        // 각 TEST_F 실행 직전에 무조건 실행
        void SetUp() override {
            motorController = std::make_unique<MotorController>(&bus, motor);

            // 앞으로 가도록 유도
			sensorProvider.left = false;
        }

    };

    TEST_F(MotorControllerPowerTest, TurnOn) {
		motorController->turnOn();

        motorController->AvoidObstacle(sensorProvider);
        motorController->MCMove();
        ASSERT_TRUE(motor.point.isEqual(Point(-1, 0)));
        ASSERT_EQ(motor.stopCount, 0);
        ASSERT_EQ(motor.forwardCount, 1);
        ASSERT_EQ(motor.leftCount, 1);

    };

    TEST_F(MotorControllerPowerTest, TurnOff) {
        motorController->turnOff();

        motorController->AvoidObstacle(sensorProvider);

		// turnOff이므로 Stop 상태 유지
        ASSERT_TRUE(motor.point.isEqual(Point(0,0)));
        ASSERT_EQ(motor.stopCount, 0);
        ASSERT_EQ(motor.forwardCount, 0);
        ASSERT_EQ(motor.backwardCount, 0);
        ASSERT_EQ(motor.leftCount, 0);
        ASSERT_EQ(motor.rightCount, 0);
    };

    class MotorControllerAvoidTest : public ::testing::Test {
    protected:
        EventBus bus;
        FakeMotor motor;
        std::unique_ptr<MotorController> motorController;
        FakeSensorProvider sensorProvider;

        // 각 TEST_F 실행 직전에 무조건 실행
        void SetUp() override {
            motorController = std::make_unique<MotorController>(&bus, motor);

             //전원 켠 상태
            motorController->turnOn();
        }
    };

    TEST_F(MotorControllerAvoidTest, FrontObstacle) {
        // 앞만 막혀 있고 왼쪽이 열려 있는 상황
		sensorProvider.left = false;

        motorController->AvoidObstacle(sensorProvider);
        motorController->MCMove();

        ASSERT_TRUE(motor.point.isEqual(Point(-1, 0)));
        ASSERT_FALSE(motor.point.isEqual(Point(0, 1)));
        ASSERT_FALSE(motor.point.isEqual(Point(1, 0)));        
        ASSERT_FALSE(motor.point.isEqual(Point(0, -1)));

        ASSERT_EQ(motor.stopCount, 0);
        ASSERT_EQ(motor.forwardCount, 1);
        ASSERT_EQ(motor.backwardCount, 0);
        ASSERT_EQ(motor.leftCount, 1);
        ASSERT_EQ(motor.rightCount, 0);
    };

    TEST_F(MotorControllerAvoidTest, LeftOpenEscapesWithoutRightSensor) {
        // 오른쪽 센서 없이도 왼쪽이 열려 있으면 좌회전 후 탈출한다.
        sensorProvider.left = false;

        motorController->AvoidObstacle(sensorProvider);
        motorController->MCMove();

        ASSERT_TRUE(motor.point.isEqual(Point(-1, 0)));
        ASSERT_EQ(motor.forwardCount, 1);
        ASSERT_EQ(motor.leftCount, 1);
        ASSERT_EQ(motor.rightCount, 0);
        ASSERT_FALSE(motorController->isAvoiding());
    };

    TEST_F(MotorControllerAvoidTest, LeftBlockedTurnsRightWithoutRightSensor) {
        // 앞과 왼쪽이 막혀 있으면 오른쪽 센서 없이 오른쪽으로 돌아 전방을 확인한다.
        sensorProvider.left = true;

        motorController->AvoidObstacle(sensorProvider);
        motorController->MCMove();

        ASSERT_TRUE(motor.point.isEqual(Point(1, 0)));
        ASSERT_EQ(motor.forwardCount, 1);
        ASSERT_EQ(motor.backwardCount, 0);
        ASSERT_EQ(motor.leftCount, 0);
        ASSERT_EQ(motor.rightCount, 1);
        ASSERT_FALSE(motorController->isAvoiding());
    };

    TEST_F(MotorControllerAvoidTest, RightCheckBlockedStartsBackwardWithoutRightSensor) {
        // 왼쪽이 막혀 우회전한 뒤에도 다시 장애물 이벤트가 오면 후진한다.
        ScriptedSensorProvider scripted;
        scripted.leftStates = {true};

        motorController->AvoidObstacle(scripted);
        motorController->AvoidObstacle(scripted);
        motorController->MCMove();

        ASSERT_EQ(motor.rightCount, 2);
        ASSERT_EQ(motor.leftCount, 1);
        ASSERT_EQ(motor.backwardCount, 1);
        ASSERT_TRUE(motor.point.isEqual(Point(0, -1)));
        ASSERT_FALSE(motorController->isAvoiding());
    };

    TEST_F(MotorControllerAvoidTest, BackwardWithLeftStillBlockedChecksRightAgain) {
        ScriptedSensorProvider scripted;
        scripted.leftStates = {true};

        motorController->AvoidObstacle(scripted);
        motorController->AvoidObstacle(scripted);
        motorController->MCMove();

        ASSERT_EQ(motor.stopCount, 0);
        ASSERT_EQ(motor.backwardCount, 1);
        ASSERT_EQ(motor.rightCount, 2);
        ASSERT_EQ(motor.leftCount, 1);
        ASSERT_EQ(motor.forwardCount, 0);

        ASSERT_TRUE(motor.point.isEqual(Point(0, -1)));
        ASSERT_FALSE(motorController->isAvoiding());
    };

    TEST_F(MotorControllerAvoidTest, SeqLeftOpen) {
        ScriptedSensorProvider scripted;
        scripted.leftStates = {true, false};

        motorController->AvoidObstacle(scripted);
        motorController->AvoidObstacle(scripted);
        motorController->MCMove();
        motorController->MCMove();

        ASSERT_EQ(motor.backwardCount, 1);
        ASSERT_EQ(motor.leftCount, 2);
        ASSERT_EQ(motor.rightCount, 1);
        ASSERT_EQ(motor.forwardCount, 1);
        ASSERT_TRUE(motor.point.isEqual(Point(-1, -1)));
    };
}
