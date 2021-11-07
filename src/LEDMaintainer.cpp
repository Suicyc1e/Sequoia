#include <LEDMaintainer.h>
#include <PIOMapper.h>

LEDMaintainer::LEDMaintainer():
    _greenLED(USER_GREEN_LED_PIO, LedColor::GREEN),
    _redLED(USER_RED_LED_PIO, LedColor::RED)
{

}

LEDMaintainer::~LEDMaintainer()
{
}


LEDMaintainer *LEDMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new LEDMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


void LEDMaintainer::SetOrangeLEDState(LedControl state)
{
    _greenLED.SetState(state);
    _redLED.SetState(state);
}


void LEDMaintainer::ToggleOrangeLED(uint32_t seconds)
{
    _greenLED.Toggle(seconds);
    _redLED.Toggle(seconds);
}


void LEDMaintainer::StopToggleOrangeLED()
{
    _greenLED.StopToggling();
    _redLED.StopToggling();
}


LEDMaintainer *LEDMaintainer::pinstance_{nullptr};
SemaphoreHandle_t LEDMaintainer::mutex_{xSemaphoreCreateMutex()};
