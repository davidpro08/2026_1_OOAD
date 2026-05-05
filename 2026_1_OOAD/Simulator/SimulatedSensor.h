#pragma once

#include "Class/ISensor.h"
#include "Class/Motor.h"
#include "Simulator/GridMap.h"

enum class SensorDirection {
    Front,
    Left,
    Right,
    Dust
};

class SimulatedSensor : public ISensor {
public:
    SimulatedSensor(GridMap* map, Motor* motor, SensorDirection direction);

    bool detect() override;
    bool peek() const;

private:
    GridMap* map;
    Motor* motor;
    SensorDirection direction;

    Point getTargetPoint() const;
};
