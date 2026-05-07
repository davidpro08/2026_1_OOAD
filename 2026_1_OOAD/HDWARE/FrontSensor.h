#pragma once

#include "Class/ISensor.h"

class FrontSensor : public ISensor {
private:
    float detectedValue = -1.0F;

public:
    bool detect() override {
        return detectedValue > 0 && detectedValue < 5;
    }

    void setFrontSensorValue(float value) {
        detectedValue = value;
    }
};
