#include "Simulator/SimulatedSensor.h"

SimulatedSensor::SimulatedSensor(GridMap* map, Motor* motor, SensorDirection direction)
    : map(map), motor(motor), direction(direction) {
}

bool SimulatedSensor::detect() {
    return peek();
}

bool SimulatedSensor::peek() const {
    if (!map || !motor) {
        return false;
    }

    if (direction == SensorDirection::Dust) {
        return map->hasDust(motor->point);
    }

    return map->isWall(getTargetPoint());
}

Point SimulatedSensor::getTargetPoint() const {
    Point vector = motor->direction;

    if (direction == SensorDirection::Left) {
        vector = Point(-motor->direction.y, motor->direction.x);
    } else if (direction == SensorDirection::Right) {
        vector = Point(motor->direction.y, -motor->direction.x);
    }

    return Point(motor->point.x + vector.x, motor->point.y + vector.y);
}
