#pragma once

#include "Class/Motor.h"
#include "Simulator/GridMap.h"

class SimulatedMotor : public Motor {
public:
    explicit SimulatedMotor(GridMap* map);

    void moveForward() override;
    void moveBackward() override;
    bool wasBlocked() const;
    void clearBlocked();
    void resetPose(Point point, Point direction);

private:
    GridMap* map;
    bool blocked;

    bool tryMove(Point next);
};
