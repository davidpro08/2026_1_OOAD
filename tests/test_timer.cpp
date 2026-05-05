#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/CleanerController.h"
#include "Class/Timer.h"
#include <future>
#include <atomic>

#include <gtest/gtest.h>

//Timer를 상속받은 testClass 생성(private까지 확인하기 위해)

//setTimer 시 current_time 변화 확인
TEST(TimerTest, setTimerCurrentTimeCheck) {
    Timer testTimer;
        
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
    testTimer.setTimer(3, NULL);
    EXPECT_GT(testTimer.getCurrent_Time(), 0);
    std::this_thread::sleep_for(std::chrono::seconds(4));   //3초뒤 powerDown
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
}

//setTimer 시 Thread 생성 확인
TEST(TimerTest, TimerThreadCreateCheck) {
    Timer testTimer;

    testTimer.setTimer(3, NULL);
    EXPECT_EQ(testTimer.getWorkerRunning(), true);
    std::this_thread::sleep_for(std::chrono::seconds(1));   //1초뒤까지 thread 동작
    EXPECT_EQ(testTimer.getWorkerRunning(), true);
    std::this_thread::sleep_for(std::chrono::seconds(3));   //3초뒤 thread join
    EXPECT_EQ(testTimer.getWorkerRunning(), false);
}

//setTimer 시 DoTimer 실행 확인
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

//setTimer의 람다함수 적용 여부 확인
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

//이미 Thread가 있을때 SetTimer시 Current_Time 변화 확인
TEST(TimerTest, AlreadySetTimerExecutionCheck) {
    Timer testTimer;

    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
    testTimer.setTimer(3, NULL);
    EXPECT_GT(testTimer.getCurrent_Time(), 2000);   //Current_Time > 1000 체크
    std::this_thread::sleep_for(std::chrono::seconds(1));
    testTimer.setTimer(5, NULL);
    EXPECT_GT(testTimer.getCurrent_Time(), 4000);   //Current_Timer > 4000 체크
    std::this_thread::sleep_for(std::chrono::seconds(6));
    EXPECT_EQ(testTimer.getCurrent_Time(), 0);
}

//이미 Thread가 있을때 SetTimer시 다중 Thread 생성되지 않는지 확인
