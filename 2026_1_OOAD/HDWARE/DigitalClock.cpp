#include "Class/timer.h"

class DigitalClock {
private:
    int current_Time =0;
    Timer timer;
public:
    
    void Update() {
    current_Time++;
    }

    int getCurrentTime() {
        return current_Time;
    }
};
