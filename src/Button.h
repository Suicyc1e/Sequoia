#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <functional>

using ButtonPushedCallback = std::function<void(bool)>;

enum class ButtonControl
{
    ACTIVE_LOW,
    ACTIVE_HIGH
};



class Button
{

private:
    
    uint32_t _gpioNumber;    
    String _humanRedableFunction;
    
public:

    uint32_t GetGPIONumber() { return _gpioNumber;}


public:

    Button(uint32_t gpioNumber, const String& functionDescription);
    ~Button();
};




#endif