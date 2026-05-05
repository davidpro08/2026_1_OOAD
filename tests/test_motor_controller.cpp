#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"

#include <gtest/gtest.h>
#include <memory>

namespace MotorControllerTest {

    class FakeSensorProvider : public SensorProvider {
    public:
        bool left = true;
        bool right = true;

        bool getLeftState() override { return left; }
        bool getRightState() override { return right; }
    };

    class Point {
        public:
        int x;
        int y;
		Point(int x, int y) : x(x), y(y) {}
        bool isEqual(Point other) {
            return this->x == other.x && this->y == other.y;
		}
    };

    class FakeMotor : public Motor {

    public:
		Point point = Point(0, 0);
		// 초기 방향은 위쪽으로 가도록 설정
		Point direction = Point(0, 1);
        void stop() {  }
        void moveForward() { 
            point.x = point.x + direction.x; 
			point.y = point.y + direction.y;
        }
        void turnLeft() {
			Point temp = direction;
			direction.x = -temp.y;
			direction.y = temp.x;
        }
        void turnRight() {
			Point temp = direction;
			direction.x = temp.y;
			direction.y = -temp.x;
        }
        void moveBackward() {
            point.x = point.x + direction.x;
            point.y = point.y + direction.y;
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
            sensorProvider.right = false;
        }
    };

    TEST_F(MotorControllerPowerTest, TurnOn) {
		motorController->turnOn();

        motorController->AvoidObstacle(sensorProvider);
        
        // turnOff이므로 Stop 상태 유지
        ASSERT_TRUE(motor.point.isEqual(Point(1, 0)));

        ASSERT_FALSE(motor.direction.isEqual(Point(0, 0)));
    };

    TEST_F(MotorControllerPowerTest, TurnOff) {
        motorController->turnOff();

        motorController->AvoidObstacle(sensorProvider);

		// turnOff이므로 Stop 상태 유지
        ASSERT_TRUE(motor.point.isEqual(Point(0,0)));

        ASSERT_FALSE(motor.direction.isEqual(Point(1, 0)));
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
        // 앞만 막혀있는 상황
		sensorProvider.left = false;
		sensorProvider.right = false;

        motorController->AvoidObstacle(sensorProvider);

        ASSERT_TRUE(motor.point.isEqual(Point(1, 0)));
        ASSERT_FALSE(motor.point.isEqual(Point(0, 1)));
        ASSERT_FALSE(motor.point.isEqual(Point(-1, 0)));        
        ASSERT_FALSE(motor.point.isEqual(Point(0, -1)));
    };

    TEST_F(MotorControllerAvoidTest, RightObstacle) {
        // 앞, 오른쪽이 막혀있는 상황
        sensorProvider.left = false;
        sensorProvider.right = true;

        motorController->AvoidObstacle(sensorProvider);

        ASSERT_TRUE(motor.point.isEqual(Point(-1, 0)));
        ASSERT_FALSE(motor.point.isEqual(Point(0, 1)));
        ASSERT_FALSE(motor.point.isEqual(Point(1, 0)));
        ASSERT_FALSE(motor.point.isEqual(Point(0, -1)));
    };

    TEST_F(MotorControllerAvoidTest, LeftObstacle) {
        // 앞, 왼쪽이 막혀있는 상황
        sensorProvider.left = true;
        sensorProvider.right = false;

        motorController->AvoidObstacle(sensorProvider);

        ASSERT_TRUE(motor.point.isEqual(Point(1, 0)));
        ASSERT_FALSE(motor.point.isEqual(Point(0, 1)));
        ASSERT_FALSE(motor.point.isEqual(Point(-1, 0)));
        ASSERT_FALSE(motor.point.isEqual(Point(0, -1)));
    };

}
