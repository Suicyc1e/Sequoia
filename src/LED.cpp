#include <LED.h>
#include <GlobalPresets.h>


LED::LED(uint32_t ledPIO, LedColor color): 
    _gpioNumber(ledPIO),
    _color(color)
{
    pinMode(_gpioNumber, GPIO_MODE_OUTPUT);
    _isToggling = false;
    _toggleHandler = NULL;
    //Start with LED disabled
    digitalWrite(_gpioNumber, static_cast<bool>(LedControl::LED_DISABLED));
}


void LED::SetState(LedControl state)
{
    digitalWrite(_gpioNumber, static_cast<bool>(state));
}


void LED::Toggle(uint32_t seconds)
{
    if (IsToggling())
    {
        Serial.println("Already toggling! Can't do more.");
        //TODO:: Add time to current Task???
        return;
    }

    CreateRTOSToggleTask(seconds);
}


bool LED::CreateRTOSToggleTask(uint32_t secondsToRun)
{
    _secondsToRun = secondsToRun;
    xTaskCreate(ToggleTaskStart, "LedToggleTask", 5000, this, tskNO_AFFINITY, &_toggleHandler);
    return true;
}


void LED::ToggleTaskStart(void *taskStartParameters)
{
    LED *moduleObject = (LED *)taskStartParameters;
    moduleObject->ToggleHandler(moduleObject->_secondsToRun);
}


void LED::ToggleHandler(uint32_t secondsToRun)
{
    _isToggling = true;
    //this is internal value to be passed to task.
    //clear it here
    _secondsToRun = 0;

    uint32_t millisecondsToWait = 1000*secondsToRun;
    unsigned long startTime = millis();    

    //allways enable LED before Toggling
    digitalWrite(_gpioNumber, static_cast<bool>(LedControl::LED_ENABLED));
    
    uint32_t counter = 0;
    while ((millis() - startTime) <= millisecondsToWait)
    {
        vTaskDelay(pdMS_TO_TICKS(LED_TOGGLE_INTERVAL_MS));
        if (++counter & 1)
        {
            digitalWrite(_gpioNumber, static_cast<bool>(LedControl::LED_ENABLED));
        }
        else
        {
            digitalWrite(_gpioNumber, static_cast<bool>(LedControl::LED_DISABLED));
        }       
    }
    
    _isToggling = false;
    vTaskDelete(NULL); 
}

void LED::StopToggling()
{
    if (!IsToggling())
    {
        Serial.println("There's nothing to Stop. Process is not running.");
        return;
    }

    if ( _toggleHandler != NULL)
        vTaskDelete(_toggleHandler);

    _isToggling = false;
}