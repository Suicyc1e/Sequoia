#include <Sensor.h>


Sensor::Sensor(uint32_t gpioNumber, const String& functionDescription, bool isAnalog):
    _gpioNumber(gpioNumber),
    _humanRedableFunction(functionDescription)
{
    if (!isAnalog)
    {
        Serial.printf("DEBUG: Setting pinMode for gpio: %d", _gpioNumber);
        pinMode(_gpioNumber, INPUT_PULLUP);
    }        
}

Sensor::~Sensor()
{
}