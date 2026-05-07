#include "Simulator/SimulatedSensor.h"
#include "Simulator/SimulatedMotor.h"

SimulatedSensor::SimulatedSensor(GridMap* map, SimulatedMotor* motor, SensorDirection direction)
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
        return map->hasDust(motor->getPosition());
    }

    return map->isWall(getTargetPoint());
}

Point SimulatedSensor::getTargetPoint() const {
    Point facing = motor->getFacing();

    if (direction == SensorDirection::Left) {
        facing = Point(-facing.y, facing.x);
    } else if (direction == SensorDirection::Right) {
        facing = Point(facing.y, -facing.x);
    }

    Point pos = motor->getPosition();
    return Point(pos.x + facing.x, pos.y + facing.y);
}
