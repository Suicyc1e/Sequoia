#ifndef SMART_DOOR_BELL_H
#define SMART_DOOR_BELL_H

#include "WString.h"
#include <Arduino.h>
#include <ISequoiaDevice.h>
#include <CameraCommandLineProcessor.h>
#include <ZigBeeCommandLineProcessor.h>


class SmartDoorBell : public virtual ISequoiaDevice
{

//overriding
public:

    virtual void SetNetworkCredentials(const NetworkCredentials& credentials) override final;

private:

    NetworkCredentials _networkCredentials;

//Refactor ->>> Dependency Injection. Or may be like a "Service" approach.
//Singletoning)))
private:

    static SmartDoorBell *pinstance_;
    static SemaphoreHandle_t mutex_;

private:
    /* data */

    //Controls

    bool        _autoanswer;
    bool        _soundDetect;
    bool        _photoSaving;
    bool        _soundSaving;
    bool        _buttonTriggered;
    bool        _microPhoneTriggered;
    bool        _boxVibrationTriggered;
    uint32_t    _additionalSensorMapper;

    bool        _webInterfaceEnabled;


    //ZigBee Network

    bool _zigBeeIsConnected;

    //WiFi Network

    IPAddress _ownIPAddress;
    IPAddress _cameraIPAddress;

public:

    void SetOwnIPAddress(IPAddress givenAddress) { _ownIPAddress = givenAddress; }
    IPAddress GetOwnIPAddress() const { return _ownIPAddress; }

//Synchronization
private:

    SemaphoreHandle_t _dataAccessMutex;

//Services. Yeah, Such a monstrous class!
private:

    ZigBeeCommandLineProcessor _zigBeeCommandLineProcessor;
    CameraCommandLineProcessor _cameraCommandLineProcessor;

//ZigBee Callback Handlers

    void OnAttributeChangedHandler(ZigBeeMessage_AttributeChanged msg);
    void OnNwkSuccededHandler(ZigBeeMessage_NwkSuccess msg);
    void OnNwkFailedHandler(ZigBeeMessage_NwkFailed msg);

//Camera Callback Handlers

    void OnCameraReadyHandler(bool ready);
    void OnCameraIPSetHandler(IPAddress address);

public:

    bool GetAutoAnswer() const { return _autoanswer; }
    bool GetSoundDetect() const { return _soundDetect; }
    bool GetPhotoSaving() const { return _photoSaving; }
    bool GetSoundSaving() const { return _soundSaving; }
    bool GetButtonTriggered() const { return _buttonTriggered; }
    bool GetMicrophoneTriggered() const { return _microPhoneTriggered; }
    bool GetBoxVibrationTriggered() const { return _boxVibrationTriggered; }
    uint32_t GetAdditionalSensorMaper() const { return _additionalSensorMapper; }
    bool GetWebInterfaceEnabled() const { return _webInterfaceEnabled; }

private:

    void SetAutoAnswer(bool autoAnswer);
    void SetSoundDetect(bool soundDetect);
    void SetPhotoSaving(bool photoSaving);
    void SetSoundSaving(bool soundSaving);
    void SetButtonTriggered(bool buttonTriggered);
    void SetMicrophoneTriggered(bool microPhoneTriggered);
    void SetBoxVibrationTriggered(bool boxVibrationTriggered);
    void SetAdditionalSensorsMapper(uint32_t additionalSensorsMapper);
    void SetWebInterfaceEnabled(bool webInterfaceEnabled);

//ZigBee Processing
public:

    void AttributeChangedProcessor(uint32_t attributeID, uint32_t attributeValue, AttributeChangeSource source);

    void StartZigBee();

private:

    void InitServices();

    void StartSpeakerLoop();

//Looped Services (for the main loop())
public:

    virtual void StartLoopedServices() override final;

public:

    void PerformStartUpTests();

private:

    bool CanBeZigBeeSideChanged(uint32_t attributeID);

protected:

    SmartDoorBell();
    ~SmartDoorBell();

public:

    SmartDoorBell(SmartDoorBell &other) = delete;
    void operator=(const SmartDoorBell &) = delete;

    static SmartDoorBell *GetInstance();
};



#endif