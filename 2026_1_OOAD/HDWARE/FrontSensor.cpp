#include "Class/ISensor.h"

class FrontSensor : public ISensor {
private:
    float detectedValue=-1.0;
public:    
    bool detect() override {
        if (detectedValue > 0 && detectedValue < 5) {
            return true;
        }
        return false;
    }
    void setFrontSensorValue(float value) {
        detectedValue = value;
    }//이거랑  detect를 메인이 틱마다 부르면 될듯?
    //아니면 다르게 구현하던가 해야할듯 인터럽트라서
};