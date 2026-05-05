#include "Class/Isensor.h"

class RightSensor : public ISensor {
private:
    float detectedValue=-1.0;
public:    
    bool detect() override {
        if (detectedValue > 0 && detectedValue < 5) {
            return true;
        }
        return false;
    }
    void setRightSensorValue(float value) {
        detectedValue = value;
    }//이거를 메인이 틱마다 부르면 될듯?
    

};