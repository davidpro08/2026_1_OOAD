#pragma once

#include "HDWARE/Motor.h"
#include "Simulator/GridMap.h"

class SimulatedMotor : public Motor {
public:
    explicit SimulatedMotor(GridMap* map);

    Point getPosition() const;
    Point getFacing() const;

    void moveForward() override;
    void moveBackward() override;
    void turnLeft() override;
    void turnRight() override;
    bool wasBlocked() const;
    void clearBlocked();
    void resetPose(Point newPosition, Point newFacing);

private:
    GridMap* map;
    bool blocked;
    Point position;
    Point facing;

    bool tryMove(Point next);
};
