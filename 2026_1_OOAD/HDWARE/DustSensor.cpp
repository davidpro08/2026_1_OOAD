#include "Class/Isensor.h"

class DustSensor : public ISensor {
private:
    float detectedValue=-1.0;
public:    
    bool detect() override {
        if (detectedValue > 5) {
            return true;
        }
        return false;
    }
    void setDustSensorValue(float value) {
        detectedValue = value;
    }//이거랑 메인이 틱마다 부르면 될듯?
    

};