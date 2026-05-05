#include "Simulator/RvcSimulator.h"

#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <thread>

TEST(RvcSimulatorTest, StepMovesForwardWhenFrontIsEmpty) {
    RvcSimulator simulator;

    simulator.turnOn();
    simulator.step();

    EXPECT_TRUE(simulator.getRobotPoint().isEqual(Point(1, 3)));
    EXPECT_TRUE(simulator.isCleanerOn());
}

TEST(RvcSimulatorTest, FrontWallDoesNotEnterWallAndAvoidsObstacle) {
    RvcSimulator simulator;

    simulator.turnOn();
    simulator.toggleWall(1, 3);
    simulator.step();

    EXPECT_FALSE(simulator.getRobotPoint().isEqual(Point(1, 3)));
    EXPECT_TRUE(simulator.getRobotPoint().isEqual(Point(2, 2)));
}

TEST(RvcSimulatorTest, CleanerRemovesDustOnCurrentCell) {
    RvcSimulator simulator;

    simulator.addDust(1, 2);
    simulator.turnOn();

    EXPECT_NE(simulator.render().find('x'), std::string::npos);
}

TEST(RvcSimulatorTest, DustTriggersPowerUpAndTimerRestoresPower) {
    RvcSimulator simulator;

    simulator.addDust(1, 3);
    simulator.turnOn();
    simulator.step();

    EXPECT_TRUE(simulator.isPowerUp());
    std::this_thread::sleep_for(std::chrono::seconds(6));
    EXPECT_FALSE(simulator.isPowerUp());
}

TEST(RvcSimulatorTest, AutoStepRunsRequestedTicks) {
    RvcSimulator simulator;

    simulator.turnOn();
    simulator.autoStep(3);

    EXPECT_TRUE(simulator.getRobotPoint().isEqual(Point(1, 5)));
}
