#include <ClampMaintainer.h>


ClampMaintainer::ClampMaintainer():
    _clampPIO(CLAMP_PIO)
{
    _isFeeding = false;
    Init();
}

ClampMaintainer::~ClampMaintainer()
{
    
}



void ClampMaintainer::Init()
{
    
    pinMode(_clampPIO, GPIO_MODE_OUTPUT);
    digitalWrite(_clampPIO, 0);
}

void ClampMaintainer::Feed(uint32_t ms)
{
    if (_isFeeding)
    {
        Serial.println("Already feeding! Can't do more.");
        //TODO:: Add time to current Task???
        return;
    }

    CreateRTOSFeedTask(ms);
}


bool ClampMaintainer::CreateRTOSFeedTask(uint32_t msToRun)
{
    _msToRun = msToRun;
    xTaskCreate(FeedTaskStart, "FeedTask", 5000, this, tskNO_AFFINITY, &_feedHandler);
    return true;
}


void ClampMaintainer::FeedTaskStart(void *taskStartParameters)
{
    //ClampMaintainer *moduleObject = (ClampMaintainer *)taskStartParameters;
    //moduleObject->ToggleHandler(moduleObject->_msToRun);
    ClampMaintainer::GetInstance()->FeedHandler();
    vTaskDelete(NULL);
}

void ClampMaintainer::FeedHandler()
{
    _isFeeding = true;
    bool opened = 1; 
    bool closed = 0;
    Serial.printf("Time to wait: %d\n", _msToRun);
    digitalWrite(_clampPIO, opened);
    vTaskDelay(pdMS_TO_TICKS(_msToRun));
    digitalWrite(_clampPIO, closed);
    Serial.printf("Done! %d\n", _msToRun);
    _isFeeding = false;
}


ClampMaintainer *ClampMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new ClampMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


ClampMaintainer *ClampMaintainer::pinstance_{nullptr};
SemaphoreHandle_t ClampMaintainer::mutex_{xSemaphoreCreateMutex()};