#ifndef BUTTONS_MAINTAINER_H
#define BUTTONS_MAINTAINER_H

#include <Arduino.h>
#include <Button.h>

class ButtonsMaintainer
{
//Singletoning)))
private:

    static ButtonsMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

protected:

    ButtonsMaintainer(/* args */);
    ~ButtonsMaintainer();

private:

    Button _mainAndOnlyButton;

private:

    void InitializeInterrupts();
    static void IRAM_ATTR MainButtonReaderISR();
    static uint32_t TimesCliked;

public:

    void MainButtonInterruptHandler();

private:

    //TODO:: MACRO IT!
    bool CreateRTOSInterruptCallbackTask();
    static void InterruptCallbackTaskStart(void *taskStartParameters);

public:

    void MainButtonCallBackTaskHandler();

public:

    ButtonsMaintainer(ButtonsMaintainer &other) = delete;
    void operator=(const ButtonsMaintainer &) = delete;

    static ButtonsMaintainer *GetInstance();

};




#endif