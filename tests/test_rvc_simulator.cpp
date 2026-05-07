#include "Simulator/RvcSimulator.h"

#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <thread>

TEST(RvcSimulatorTest, StepMovesForwardWhenFrontIsEmpty) {
    RvcSimulator simulator;

    simulator.turnOn();
    simulator.step();

    EXPECT_TRUE(simulator.getRobotPoint().isEqual(Point(1, 2)));
    EXPECT_TRUE(simulator.isCleanerOn());
}

//TEST(RvcSimulatorTest, FrontWallDoesNotEnterWallAndAvoidsObstacle) {
//    RvcSimulator simulator;
//
//    simulator.turnOn();
//    simulator.toggleWall(1, 3);
//    simulator.step();
//
//    EXPECT_FALSE(simulator.getRobotPoint().isEqual(Point(1, 3)));
//    EXPECT_TRUE(simulator.getRobotPoint().isEqual(Point(2, 2)));
//}

TEST(RvcSimulatorTest, DustTriggersPowerUpAndTimerRestoresPower) {
    RvcSimulator simulator;

    simulator.addDust(1, 2);
    simulator.turnOn();
    simulator.step();

    EXPECT_TRUE(simulator.isPowerUp());

    for (int i = 0; i < 5; ++i) {
        simulator.step();
    }

    EXPECT_FALSE(simulator.isPowerUp());
}

TEST(RvcSimulatorTest, TurnOffStopsCleaner) {
    RvcSimulator simulator;

    simulator.turnOn();
    ASSERT_TRUE(simulator.isCleanerOn());

    simulator.turnOff();

    EXPECT_FALSE(simulator.isPowerOn());
    EXPECT_FALSE(simulator.isCleanerOn());
}

TEST(RvcSimulatorTest, AutoStepRunsRequestedTicks) {
    RvcSimulator simulator;

    simulator.turnOn();
    simulator.autoStep(3);

    EXPECT_TRUE(simulator.getRobotPoint().isEqual(Point(1, 4)));
}
