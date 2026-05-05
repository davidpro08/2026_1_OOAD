#pragma once

#include "Class/HwCleaner.h"
#include "Class/Motor.h"
#include "Simulator/GridMap.h"

class SimulatedCleaner : public HwCleaner {
public:
    SimulatedCleaner(GridMap* map, Motor* motor);

    void powerUp() override;
    void powerRestore() override;
    void turnOn() override;
    void turnOff() override;

    bool isTurnedOn() const;
    bool isPowerUp() const;
    bool cleanCurrentCell();
    void reset();

private:
    GridMap* map;
    Motor* motor;
    bool turnedOn;
    bool powerUpMode;
};
