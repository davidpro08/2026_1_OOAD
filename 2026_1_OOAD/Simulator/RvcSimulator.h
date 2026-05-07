#pragma once

#include "Class/CleanerController.h"
#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/PowerController.h"
#include "Class/SensorController.h"
#include "Simulator/GridMap.h"
#include "Simulator/SimulatedCleaner.h"
#include "Simulator/SimulatedMotor.h"
#include "Simulator/SimulatedSensor.h"

#include <cstdint>
#include <memory>
#include <string>

class RvcSimulator {
public:
    RvcSimulator();
    ~RvcSimulator();

    void run();
    void reset();
    void resetRandomMap(uint32_t seed = 0);
    void turnOn();
    void turnOff();
    void step();
    void autoStep(int count);
    void forceFrontObstacle();
    bool addDust(int x, int y);
    bool toggleWall(int x, int y);
    std::string statusText() const;
    std::string render() const;

    Point getRobotPoint() const;
    Point getRobotDirection() const;
    bool isPowerOn() const;
    bool isCleanerOn() const;
    bool isPowerUp() const;

private:
    GridMap map;
    EventBus bus;
    SimulatedMotor motor;
    SimulatedSensor frontSensor;
    SimulatedSensor leftSensor;
    SimulatedSensor rightSensor;
    SimulatedSensor dustSensor;
    SimulatedCleaner cleaner;
    SensorController sensorController;
    MotorController motorController;
    CleanerController cleanerController;
    PowerController powerController;
    bool powerOn;

    void printScreen() const;
    void printHelp() const;
    bool handleCommand(const std::string& line);
    std::string directionText() const;
};
