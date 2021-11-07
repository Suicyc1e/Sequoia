#ifndef CLAMP_MAINTAINER
#define CLAMP_MAINTAINER

#include <Arduino.h>
#include <PIOMapper.h>

class ClampMaintainer
{
private:

    static ClampMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

protected:

    ClampMaintainer(/* args */);
    ~ClampMaintainer();

private:

    uint32_t _clampPIO;
    bool _isFeeding;
    uint32_t _msToRun;
    TaskHandle_t _feedHandler;

public:

    void Feed(uint32_t ms);

private:

    void FeedHandler();

private:

    bool CreateRTOSFeedTask(uint32_t msToRun);
    static void FeedTaskStart(void *taskStartParameters);

public:

    void TurnPIOOff() { digitalWrite(_clampPIO, 0); }
    void TurnPIOON() { digitalWrite(_clampPIO, 1); }

private:

    void Init();

public:

    ClampMaintainer(ClampMaintainer &other) = delete;
    void operator=(const ClampMaintainer &) = delete;

    static ClampMaintainer *GetInstance();

};



#endif