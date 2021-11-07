#ifndef LED_MAINATINER_H
#define LED_MAINTAINER_H

#include <Arduino.h>
#include <LED.h>


class LEDMaintainer
{

//Singletoning)))
private:

    static LEDMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

protected:

    LEDMaintainer(/* args */);
    ~LEDMaintainer();

private:

    LED _greenLED;
    LED _redLED;


public:

    void SetGreenLEDState(LedControl state) { _greenLED.SetState(state); }
    void SetRedLEDState(LedControl state) { _redLED.SetState(state); }

    void ToggleGreenLED(uint32_t seconds) { _greenLED.Toggle(seconds); }
    void ToggleRedLED(uint32_t seconds) { _redLED.Toggle(seconds); }

    void StopToggleGreenLED() { _greenLED.StopToggling(); }
    void StopToggleRedLED() { _redLED.StopToggling(); }

    //Make Orange?
    void SetOrangeLEDState(LedControl state);
    void ToggleOrangeLED(uint32_t seconds);
    void StopToggleOrangeLED();

public:

    LEDMaintainer(LEDMaintainer &other) = delete;
    void operator=(const LEDMaintainer &) = delete;

    static LEDMaintainer *GetInstance();


};

#endif