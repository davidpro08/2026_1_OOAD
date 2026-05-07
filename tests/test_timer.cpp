#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/CleanerController.h"
#include "Class/Timer.h"
#include <future>
#include <atomic>

#include <gtest/gtest.h>

//Timer�� ��ӹ��� testClass ����(private���� Ȯ���ϱ� ����)

//setTimer �� current_time ��ȭ Ȯ��
TEST(TimerTest, setTimerCurrentTimeCheck) {
    Timer testTimer;
        
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
    testTimer.setTimer(3, NULL);
    EXPECT_GT(testTimer.getCurrent_Time(), 0);
    std::this_thread::sleep_for(std::chrono::seconds(4));   //3�ʵ� powerDown
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
}

//setTimer �� Thread ���� Ȯ��
TEST(TimerTest, TimerThreadCreateCheck) {
    Timer testTimer;

    testTimer.setTimer(3, NULL);
    EXPECT_EQ(testTimer.getWorkerRunning(), true);
    std::this_thread::sleep_for(std::chrono::seconds(1));   //1�ʵڱ��� thread ����
    EXPECT_EQ(testTimer.getWorkerRunning(), true);
    std::this_thread::sleep_for(std::chrono::seconds(3));   //3�ʵ� thread join
    EXPECT_EQ(testTimer.getWorkerRunning(), false);
}

//setTimer �� DoTimer ���� Ȯ��
TEST(TimerTest, DoTimerExecutionCheck) {
    Timer testTimer;

    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
    testTimer.setTimer(3, NULL);
    EXPECT_LE(testTimer.getCurrent_Time(), 3000);
    std::this_thread::sleep_for(std::chrono::seconds(2));  
    EXPECT_LE(testTimer.getCurrent_Time(), 1500);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
}

//setTimer�� �����Լ� ���� ���� Ȯ��
/*
TEST(TimerTest, LamdaOutputExecutionCheck) {
    Timer testTimer;
    std::atomic<int> test_count = 0;

    Timer timer;
    timer.setTimer(0, [&]() {test_count++;});


    EXPECT_EQ(test_count, 0);
    timer.setTimer(0, [&]() {test_count++;});
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(test_count, 1);
    for (int i = 0; i < 3; i++) {
        timer.setTimer(0, [&]() {test_count++;});
    }
    std::this_thread::sleep_for(std::chrono::seconds(30));
    EXPECT_EQ(test_count, 4);
}*/

//�̹� Thread�� ������ SetTimer�� Current_Time ��ȭ Ȯ��
TEST(TimerTest, AlreadySetTimerExecutionCheck) {
    Timer testTimer;

    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
    testTimer.setTimer(3, NULL);
    EXPECT_GT(testTimer.getCurrent_Time(), 2000);   //Current_Time > 1000 üũ
    std::this_thread::sleep_for(std::chrono::seconds(1));
    testTimer.setTimer(5, NULL);
    EXPECT_GT(testTimer.getCurrent_Time(), 4000);   //Current_Timer > 4000 üũ
    std::this_thread::sleep_for(std::chrono::seconds(6));
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
}

//�̹� Thread�� ������ SetTimer�� ���� Thread �������� �ʴ��� Ȯ��
