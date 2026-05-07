#pragma once

#include "Class/ISensor.h"
#include "Simulator/GridMap.h"

class SimulatedMotor;

enum class SensorDirection {
    Front,
    Left,
    Right,
    Dust
};

class SimulatedSensor : public ISensor {
public:
    SimulatedSensor(GridMap* map, SimulatedMotor* motor, SensorDirection direction);

    bool detect() override;
    bool peek() const;

private:
    GridMap* map;
    SimulatedMotor* motor;
    SensorDirection direction;

    Point getTargetPoint() const;
};
