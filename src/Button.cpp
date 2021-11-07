#include <Button.h>

Button::Button(uint32_t gpioNumber, const String& functionDescription):
    _gpioNumber(gpioNumber),
    _humanRedableFunction(functionDescription)
{
    //There should be switch regarding type of button...
    pinMode(_gpioNumber, INPUT_PULLUP);
}

Button::~Button()
{
}