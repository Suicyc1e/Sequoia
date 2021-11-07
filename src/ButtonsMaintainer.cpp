#include <ButtonsMaintainer.h>
#include <PIOMapper.h>
#include <TesterOfTheFeatures.h>
#include <GlobalPresets.h>

ButtonsMaintainer::ButtonsMaintainer():
    _mainAndOnlyButton(USER_BUTTON_PIO, "MainButton")
{
    //Set Interrupt Handler HERE!
    InitializeInterrupts();
}

void ButtonsMaintainer::InitializeInterrupts()
{
    attachInterrupt(_mainAndOnlyButton.GetGPIONumber(), MainButtonReaderISR, FALLING);
}

void ButtonsMaintainer::MainButtonReaderISR()
{   
    //debounce
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > BUTTON_DEBOUNCE_INTERVAL_MS)
    {
        ++TimesCliked;
        Serial.printf("Main Button Clicked: %d\n", TimesCliked);

        ButtonsMaintainer::GetInstance()->MainButtonInterruptHandler(); 
    }
    last_interrupt_time = interrupt_time;
  
}


bool ButtonsMaintainer::CreateRTOSInterruptCallbackTask()
{
    xTaskCreate(InterruptCallbackTaskStart, "LedToggleTask", 5000, this, tskNO_AFFINITY, NULL);
    return true;
}


void ButtonsMaintainer::InterruptCallbackTaskStart(void *taskStartParameters)
{
    ButtonsMaintainer *moduleObject = (ButtonsMaintainer *)taskStartParameters;
    moduleObject->MainButtonCallBackTaskHandler();

    vTaskDelete(NULL);
}


void ButtonsMaintainer::MainButtonCallBackTaskHandler()
{
    Serial.println("We can do anything Async Here!");
    //Some ASYNC Job can be called here.
    TesterOfTheFeatures::GetInstance()->IncrementLedTestPhase();
}


void ButtonsMaintainer::MainButtonInterruptHandler()
{
    Serial.println("Entering HandleMainButtonInterrupt Zone");
    CreateRTOSInterruptCallbackTask();
}


ButtonsMaintainer::~ButtonsMaintainer()
{
}


ButtonsMaintainer *ButtonsMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new ButtonsMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


ButtonsMaintainer *ButtonsMaintainer::pinstance_{nullptr};
SemaphoreHandle_t ButtonsMaintainer::mutex_{xSemaphoreCreateMutex()};
uint32_t ButtonsMaintainer::TimesCliked{0};