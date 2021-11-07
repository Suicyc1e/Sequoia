#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor
{

private:
    
    uint32_t _gpioNumber;    
    String _humanRedableFunction;

public:

    uint32_t GetGPIONumber() { return _gpioNumber;}

public:

    Sensor(uint32_t gpioNumber, const String& functionDescription, bool isAnalog = false);
    ~Sensor();
};




#endif