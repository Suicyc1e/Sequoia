#ifndef MICROPHONE_MAINTAINER_H
#define MICROPHONE_MAINTAINER_H

#include <Audio.h>
#include <ADCSampler.h>

class MicrophoneMaintainer
{
//Singletoning)    
private:
    
    static MicrophoneMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;
    static SemaphoreHandle_t dataMutex_;

protected:

    MicrophoneMaintainer();
    ~MicrophoneMaintainer();

private:

    I2SSampler *_input = NULL;
    TaskHandle_t *_buttonPushHandler = NULL;

private:

    I2SSampler* GetInput() { return _input; }
    
private:

    static void IRAM_ATTR PushButtonReaderISR();
    static void WaitForButtonPush();

    static void RecordHandlerTask(void *parameter); //bool - start-stop record.

private:

    static uint32_t TimesCliked;
    uint32_t _records;

public:

    uint32_t GetRecordNumber() const { return _records; }
    void IncreaseRecordsNumber() { ++_records; } 

private:

    void MicrophoneInit();
    void TestPushButtonInit();

public:

    MicrophoneMaintainer(MicrophoneMaintainer &other) = delete;
    void operator=(const MicrophoneMaintainer &) = delete;

    static MicrophoneMaintainer *GetInstance();

};




#endif