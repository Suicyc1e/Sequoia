#ifndef LED_H
#define LED_H

#include <Arduino.h>

enum class LedColor
{
    RED,
    GREEN
};


//Change it to reverse control!
// Production (Common Cathode):
//    LED_ENABLED = 0,
//    LED_DISABLED = 1
// Testing (Single Diods):
//    LED_ENABLED = 1,
//    LED_DISABLED = 0


enum class LedControl
{
    LED_ENABLED = 0,
    LED_DISABLED = 1
};


class LED
{

private:

    uint32_t _gpioNumber;
    LedColor _color;

private:

    LedControl _state; //to not GPIO_READ each and every time.
    bool _isToggling;
    TaskHandle_t _toggleHandler;

private:

    uint32_t _secondsToRun;

public:

    void SetState(LedControl state);
    LedControl GetState() { return _state; }
    void Toggle(uint32_t seconds);
    bool IsToggling() const { return _isToggling; }
    void StopToggling();

private:

    void ToggleHandler(uint32_t secondsToRun); //0 ---> infinite cycle

private:

    bool CreateRTOSToggleTask(uint32_t secondsToRun);
    static void ToggleTaskStart(void *taskStartParameters);

public:

    LED(uint32_t ledPIO, LedColor color);

};

#endif