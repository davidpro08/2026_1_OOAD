#pragma once

#include "Class/ISensor.h"

class DustSensor : public ISensor {
private:
    float detectedValue = -1.0F;

public:
    bool detect() override {
        return detectedValue > 5;
    }

    void setDustSensorValue(float value) {
        detectedValue = value;
    }
};
