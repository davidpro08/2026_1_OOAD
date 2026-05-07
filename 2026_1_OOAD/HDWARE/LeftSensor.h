#pragma once

#include "Class/ISensor.h"

class LeftSensor : public ISensor {
private:
    float detectedValue = -1.0F;

public:
    bool detect() override {
        return detectedValue > 0 && detectedValue < 5;
    }

    void setLeftSensorValue(float value) {
        detectedValue = value;
    }
};
