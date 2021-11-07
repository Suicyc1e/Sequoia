#include <SmartDoorBell.h>
#include <functional>
#include <SDCardMaintainer.h>
#include <SpeakerMaintainer.h>
#include <ScalesMaintainer.h>
#include <MicrophoneMaintainer.h>
#include <LEDMaintainer.h>
#include <ButtonsMaintainer.h>
#include <TesterOfTheFeatures.h>
#include <ClampMaintainer.h>
#include <SensorsMaintainer.h>

//https://arduino.stackexchange.com/questions/14157/passing-class-member-function-as-argument
SmartDoorBell::SmartDoorBell() :
    _zigBeeCommandLineProcessor([&](ZigBeeMessage_AttributeChanged msg){OnAttributeChangedHandler(msg);} ,
    [&](ZigBeeMessage_NwkFailed msg){OnNwkFailedHandler(msg);},
    [&](ZigBeeMessage_NwkSuccess msg){OnNwkSuccededHandler(msg);})
{
    //Creating Data Access mutex
    _dataAccessMutex = xSemaphoreCreateMutex();

#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT
    
    InitServices();

#endif

}

void SmartDoorBell::SetNetworkCredentials(const NetworkCredentials& credentials) 
{
    _networkCredentials = credentials;
    _cameraCommandLineProcessor.SendSetSSIDCommand(credentials.m_ssid);
    _cameraCommandLineProcessor.SendSetPasswordCommand(credentials.m_pswd);    
}



void SmartDoorBell::InitServices()
{
    //TODO:: Continue here refatcoring theese TASKS!
    _zigBeeCommandLineProcessor.ExecuteStartUpProcedures();
    _cameraCommandLineProcessor.ExecuteStartUpProcedures();

    _cameraCommandLineProcessor.SetCameraCommandLineCallbacks(
        [&](bool status){ OnCameraReadyHandler(status); },
        [&](IPAddress address){ OnCameraIPSetHandler(address); }
    );

    //ClampMaintainer::GetInstance();
    SDCardMaintainer::GetInstance();         
    SpeakerMaintainer::GetInstance();
    LEDMaintainer::GetInstance();
    ButtonsMaintainer::GetInstance();
    TesterOfTheFeatures::GetInstance();
    //MicrophoneMaintainer::GetInstance();
    //ScalesMaintainer::GetInstance();
    SensorsMaintainer::GetInstance();
    

    //DEBUG!!!
    //DEBUG!!!!

}


void SmartDoorBell::PerformStartUpTests()
{
    //StartSpeakerLoop();
}

void SmartDoorBell::StartLoopedServices()
{   
    StartSpeakerLoop();
}


void SmartDoorBell::StartSpeakerLoop()
{
    SpeakerMaintainer::GetInstance()->StartAudioLoop();
}

SmartDoorBell::~SmartDoorBell()
{
}

bool SmartDoorBell::CanBeZigBeeSideChanged(uint32_t attributeID)
{
    if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::AUTO_ANSWER)
        || attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::SOUND_DETECT)
        || attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::PHOTO_SAVING)
        || attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::SOUND_SAVING)
        || attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::WEB_INTERFACE))
    {
        return true;
    }
    
    return false;
}


void SmartDoorBell::StartZigBee()
{
    _zigBeeCommandLineProcessor.SendZigBeeStartUp();
}


void SmartDoorBell::AttributeChangedProcessor(uint32_t attributeID, uint32_t attributeValue, AttributeChangeSource source)
{
    Serial.println("Entering AttributeChangedProcessor");

    if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::AUTO_ANSWER))
    {
        SetAutoAnswer(attributeValue); //can be casted to bool)
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendAutoAnswerCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::SOUND_DETECT))
    {
        SetSoundDetect(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendSoundDetectionCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::PHOTO_SAVING))
    {
        SetPhotoSaving(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendPhotoSavingCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::SOUND_SAVING))
    {
        SetSoundSaving(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendSoundSavingCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::BUTTON_TRIGGERED))
    {
        SetButtonTriggered(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendButtonTriggeredCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::MICROPHONE_TRIGGERED))
    {
        SetMicrophoneTriggered(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendMicroPhoneTriggeredCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::BOX_VIBRATION_TRIGGERED))
    {
        SetBoxVibrationTriggered(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendBoxVibrationTriggeredCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::ADDITIONAL_MAPPER_STATE))
    {
        SetAdditionalSensorsMapper(attributeValue);
        if (source != AttributeChangeSource::ZIGBEE_SIDE)
        {
            _zigBeeCommandLineProcessor.SendAdditionalMapperCommand(attributeValue);
        }
    }
    else if (attributeID == static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::WEB_INTERFACE))
    {
        SetWebInterfaceEnabled(attributeValue);
    }
    else
    {
        Serial.printf("ERROR: Unsupported Attribute ID! : %d", attributeID);
    }
    

}


void SmartDoorBell::OnAttributeChangedHandler(ZigBeeMessage_AttributeChanged msg)
{
    AttributeChangedProcessor(msg.GetAttributeId(), msg.GetNewAttributeValue(), AttributeChangeSource::ZIGBEE_SIDE);
}


void SmartDoorBell::OnNwkSuccededHandler(ZigBeeMessage_NwkSuccess msg)
{
    Serial.printf("Connected to ZigBee Network! Status: %d", msg.GetStatus());
    _zigBeeIsConnected = true;

    //TODO:: Stop LED Blinking HERE!
}


void SmartDoorBell::OnNwkFailedHandler(ZigBeeMessage_NwkFailed msg)
{
    Serial.printf("DISCONNECTED from ZigBee Network! Status: %d\n", msg.GetStatus());
    _zigBeeIsConnected = false;
}


void SmartDoorBell::OnCameraReadyHandler(bool ready)
{
    Serial.printf("Camera reports READY! Status: %d\n", ready);
}


void SmartDoorBell::OnCameraIPSetHandler(IPAddress address)
{
    Serial.printf("Camera reports IP Address:: %s\n", address.toString().c_str());
}


void SmartDoorBell::SetAutoAnswer(bool autoAnswer) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _autoanswer = autoAnswer; 
    xSemaphoreGive(_dataAccessMutex);
}

void SmartDoorBell::SetSoundDetect(bool soundDetect) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _soundDetect = soundDetect;
    xSemaphoreGive(_dataAccessMutex); 
}

void SmartDoorBell::SetPhotoSaving(bool photoSaving) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _photoSaving = photoSaving;
    xSemaphoreGive(_dataAccessMutex); 
}

void SmartDoorBell::SetSoundSaving(bool soundSaving) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _soundSaving = soundSaving;
    xSemaphoreGive(_dataAccessMutex); 
}

void SmartDoorBell::SetButtonTriggered(bool buttonTriggered) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _buttonTriggered = buttonTriggered;
    xSemaphoreGive(_dataAccessMutex); 
}

void SmartDoorBell::SetMicrophoneTriggered(bool microPhoneTriggered) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _microPhoneTriggered = microPhoneTriggered;
    xSemaphoreGive(_dataAccessMutex);  
}

void SmartDoorBell::SetBoxVibrationTriggered(bool boxVibrationTriggered) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _boxVibrationTriggered = boxVibrationTriggered;
    xSemaphoreGive(_dataAccessMutex);  
}

void SmartDoorBell::SetAdditionalSensorsMapper(uint32_t additionalSensorsMapper) 
{ 
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _additionalSensorMapper = additionalSensorsMapper;
    xSemaphoreGive(_dataAccessMutex);  
}

void SmartDoorBell::SetWebInterfaceEnabled(bool webInterfaceEnabled)
{
    xSemaphoreTake(_dataAccessMutex, portMAX_DELAY);
    _webInterfaceEnabled = webInterfaceEnabled;
    xSemaphoreGive(_dataAccessMutex);  
}

SmartDoorBell *SmartDoorBell::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SmartDoorBell();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


SmartDoorBell *SmartDoorBell::pinstance_{nullptr};
SemaphoreHandle_t SmartDoorBell::mutex_{xSemaphoreCreateMutex()};