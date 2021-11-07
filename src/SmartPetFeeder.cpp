#include <SmartPetFeeder.h>
#include <SDCardMaintainer.h>
#include <LEDMaintainer.h>
#include <TesterOfTheFeatures.h>
#include <SpeakerMaintainer.h>
#include <ScalesMaintainer.h>
#include <ClampMaintainer.h>
#include <GlobalPresets.h>

SmartPetFeeder::SmartPetFeeder() :
    _zigBeeCommandLineProcessor([&](ZigBeeMessage_AttributeChanged msg){OnAttributeChangedHandler(msg);} ,
    [&](ZigBeeMessage_NwkFailed msg){OnNwkFailedHandler(msg);},
    [&](ZigBeeMessage_NwkSuccess msg){OnNwkSuccededHandler(msg);})
{
    _zigBeeCommandLineProcessor.SetPlantCarerCommandsCallback([&](SmartPlantCarerCommand command){OnZigBeeCommandReceivedHandler(command);});
    
#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT

    HardwareInit();
    InitServices();

#endif

    _currentWeight = 0.0;

}


void SmartPetFeeder::AttributeChangedProcessor(uint32_t attributeID, uint32_t attributeValue, AttributeChangeSource source)
{
    Serial.println("Entering AttributeChangedProcessor");
    Serial.printf("Attribte id: %d, Attribute value: %d \n", attributeID, attributeValue);

}


void SmartPetFeeder::OnZigBeeCommandReceivedHandler(SmartPlantCarerCommand command)
{
    Serial.println("Entering OnZigBeeCommandReceivedHandler");
    Serial.printf("ZigBee command received: %s\n", command.GetRawCommand().c_str());
}


void SmartPetFeeder::OnAttributeChangedHandler(ZigBeeMessage_AttributeChanged msg)
{
    AttributeChangedProcessor(msg.GetAttributeId(), msg.GetNewAttributeValue(), AttributeChangeSource::ZIGBEE_SIDE);
}


void SmartPetFeeder::OnNwkSuccededHandler(ZigBeeMessage_NwkSuccess msg)
{
    Serial.printf("Connected to ZigBee Network! Status: %d", msg.GetStatus());
    _zigBeeIsConnected = true;

    //TODO:: Stop LED Blinking HERE!
}


void SmartPetFeeder::OnNwkFailedHandler(ZigBeeMessage_NwkFailed msg)
{
    Serial.printf("DISCONNECTED from ZigBee Network! Status: %d", msg.GetStatus());
    _zigBeeIsConnected = false;
}


void SmartPetFeeder::StartZigBee()
{
    _zigBeeCommandLineProcessor.SendZigBeeStartUp();
}


void SmartPetFeeder::InitServices()
{
    _zigBeeCommandLineProcessor.ExecuteStartUpProcedures();
    ClampMaintainer::GetInstance();
    SDCardMaintainer::GetInstance();
    LEDMaintainer::GetInstance();
    TesterOfTheFeatures::GetInstance();
    SpeakerMaintainer::GetInstance();
    ScalesMaintainer::GetInstance();    
}


void SmartPetFeeder::HardwareInit()
{
    //DO NOTHING
}


void SmartPetFeeder::StartLoopedServices()
{
    //SpeakerMaintainer::GetInstance()->StartAudioLoop();
    StartSpeakerLoop();
}


void SmartPetFeeder::StartSpeakerLoop()
{
    SpeakerMaintainer::GetInstance()->StartAudioLoop();
}


void SmartPetFeeder::FeedThePet(uint32_t weightInGramms)
{
    _lockControl = true;
    
    _currentWeight = ScalesMaintainer::GetInstance()->DirectGetWeight(1);
    float assignAsTare = _currentWeight;

    ClampMaintainer::GetInstance()->Feed(FEED_PROCESS_CLAMP_MS);
    vTaskDelay(pdMS_TO_TICKS(FEED_PROCESS_CLAMP_MS));
    _currentWeight = ScalesMaintainer::GetInstance()->DirectGetWeight(1);

    int currentIteration = 1;
    while 
        (
        (((_currentWeight - assignAsTare) * 1000) <= weightInGramms)
        && 
        (currentIteration < FEED_PROCESS_MAXIMUM_NUMBER_OF_ITERATIONS)
        )
    {
        ClampMaintainer::GetInstance()->Feed(FEED_PROCESS_CLAMP_MS);
        vTaskDelay(pdMS_TO_TICKS(FEED_PROCESS_CLAMP_MS));
        vTaskDelay(pdMS_TO_TICKS(FEED_PROCESS_DELAY_MS));
        _currentWeight = ScalesMaintainer::GetInstance()->DirectGetWeight(1);
        Serial.printf("Current weight: %f\n", (_currentWeight*1000));
        Serial.printf("Weight in Gramms: %d\n", weightInGramms);

        if (((_currentWeight - assignAsTare) * 1000 + FEED_TOLERANCE_GRAMMS) >= weightInGramms)
        {
            Serial.printf("Tolerance Achieved! \n");
            break;
        }
    }
     
    Serial.printf("Feed DONE!: %f", _currentWeight * 1000);

    _lockControl = false;
}


void SmartPetFeeder::FeedThePetAsync(uint32_t weightInGramms)
{
        _currentFeedWeightInGramms = weightInGramms;
        xTaskCreate(
        AsyncFeedThePetTask,          /* Task function. */
        "AsyncFeedThePetTask",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}
    
void SmartPetFeeder::AsyncFeedThePetTask(void *parameters)
{
    SmartPetFeeder *m_ptr = (SmartPetFeeder*)(parameters);
    
    SmartPetFeeder::GetInstance()->FeedThePet(m_ptr->_currentFeedWeightInGramms);

    vTaskDelete(NULL);
}


SmartPetFeeder::~SmartPetFeeder()
{
}

SmartPetFeeder *SmartPetFeeder::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SmartPetFeeder();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


SmartPetFeeder *SmartPetFeeder::pinstance_{nullptr};
SemaphoreHandle_t SmartPetFeeder::mutex_{xSemaphoreCreateMutex()};