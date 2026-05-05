#include "Simulator/SimulatedMotor.h"

SimulatedMotor::SimulatedMotor(GridMap* map) : map(map), blocked(false) {
    resetPose(Point(1, 1), Point(0, 1));
}

void SimulatedMotor::moveForward() {
    tryMove(Point(point.x + direction.x, point.y + direction.y));
}

void SimulatedMotor::moveBackward() {
    tryMove(Point(point.x - direction.x, point.y - direction.y));
}

bool SimulatedMotor::wasBlocked() const {
    return blocked;
}

void SimulatedMotor::clearBlocked() {
    blocked = false;
}

void SimulatedMotor::resetPose(Point newPoint, Point newDirection) {
    point = newPoint;
    direction = newDirection;
    blocked = false;
}

bool SimulatedMotor::tryMove(Point next) {
    if (map && map->isWall(next)) {
        blocked = true;
        return false;
    }

    point = next;
    blocked = false;
    return true;
}
